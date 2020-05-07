//----------------------------------------------------------------------------------------------------------
// SMBus.c  
//----------------------------------------------------------------------------------------------------------
//
// AUTH: Tom Hua
// DATE: March 23, 2017
//
// Target: C8051F930
//
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
// Includes and Device-Specific Parameters
//----------------------------------------------------------------------------------------------------------
#include <compiler_defs.h>
#include <C8051F930_defs.h>            // SFR declarations
#include "SMBus.h"
#include "main.h"
#include "Timers.h"
#include "math.h"
#include "utility.h"
#include "LED.h"
#include "sound.h"


//----------------------------------------------------------------------------------------------------------
// Global CONSTANTS
//----------------------------------------------------------------------------------------------------------       
#define SYSCLK        24500000		   // Internal oscillator frequency in Hz

// Target SCL clk rate: 16014 Hz to 400kHz (SYSCLK = 49 MHz )
									   // LTC3220 - SCL clock rate: between 10 kHz and 400kHz 
									   // ( Note: lower limit is from SiLabs EEPROM example code. )
								       // - tested okay at 700 kHz, failed at 800 kHz
									   // 76682 sets Timer1 so that 115200 Hz UART can be used with the same 
									   // timer.  154088 sets Timer1 so that 230400 Hz UART can be used	with
									   // the same timer.
									   // For SCL operation above 100 kHz, EXTHOLD should be cleared to 0. 
									   
#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

// Status vectors - top 4 bits only
#define  SMB_MTSTA      0xE0           // (MT) start transmitted
#define  SMB_MTDB       0xC0           // (MT) data byte transmitted
#define  SMB_MRDB       0x80           // (MR) data byte received

//----------------------------------------------------------------------------------------------------------
// Global VARIABLES
//----------------------------------------------------------------------------------------------------------
unsigned char* pSMB_DATA_IN;        // pointer for SMBus recieve data.
unsigned char SMB_SINGLEBYTE_OUT;   // holder for single byte writes.
unsigned char* pSMB_DATA_OUT;       // pointer for SMBus transmit data.
unsigned char SMB_DATA_LEN;         // num bytes to send/receive in current SMBus transfer
unsigned char BYTEADDR_Hi;          // holder for DEVICE 8-bit address to be used in next transfer.
unsigned char BYTEADDR_Lo ;         // holder for DEVICE 8-bit address to be used in next transfer.

unsigned char TargetAddr;          

bit SMB_BUSY = 0;                   // indicate when DEVICE_Read() function has claimed bus								
bit SMB_RW;                         // Software flag to indicate the direction of the current transfer.

unsigned char SendAddr_N;           // 1: causes ISR to send 8-bit address followed by slave address. 
									// 2+: causes ISR to send 16-bit address followed by slave address.
                                    // 0: // no need to provide memory address
bit SMB_REPEATSTART = 0;
bit SMB_ACKPOLL;                    // 1: causes ISR to send repeated START until slave has 
									// acknowledged address.

bit SMB_RANDOMREAD;                 // 1: causes ISR to send START signal after sending memory address. 
									// A random read starts as a write then changes to a read 
									// after the repeated start is sent.	 The ISR handles this switchover 
									// if the <SMB_RANDOMREAD> bit is set.		  

static volatile unsigned char Melexis_Payload[4];	// Buffer for the transmitted bytes
unsigned char SMB_Payload_buf[10] = {0xE1, 0xE1};   // xmit buffer

sbit SDA = P0^2;                    // SMBus
sbit SCL = P0^3; 					// SMBus
sbit P04 = P0^4;					// for testing with the oscilloscope
sbit P05 = P0^5;					// for testing with the oscilloscope
sbit SW1 = P0^6;        			// Port 0.6 is SW1 - Hall Effect Detecter 1
sbit SW2 = P0^7;        			// Port 0.7 is SW2 - Hall Effect Detecter 2


//----------------------------------------------------------------------------------------------------------	
// Function Name:   SMBus_Init( )
//
// Description:     This routine configures the SMBus Configuration Register
//----------------------------------------------------------------------------------------------------------
void SMBus_Init (void) {
    unsigned long freq = SMB_FREQUENCY;
    SMB0CF = 0x5F;   // bit6: Disable slave mode.
                     // bit4 = 1: Enable setup & hold time extensions when SysClk > 10 MHz
                     // Others: Enable SMBus Free timeout detect. Enable SCL low timeout detect;
                     // Use Timer 2 Low Byte Overflows as SMBus clock source.
                                   
    if ( freq > 100000 )
        SMB0CF &= ~0x10;		// EXTHOLD: Disable Extended Setup and Hold Times for SMBus 
                              // frequencies over 100 kHz.
    EIP1 |= 1;                // high priority
    SMB0CF |= 0x80;             // Enable SMBus;
    SMB0CF &= ~0x80;                 // Reset communication
    SMB0CF |= 0x80;
    STA = 0;
    STO = 0;
    ACK = 0;
    
    EIE1 |= 0x01;				// Enable the SMBus interrupt   
}

//----------------------------------------------------------------------------------------------------------
// Function Name:   Timer3_ISR ()
//
// Description:
//
// A Timer3 interrupt indicates an SMBus SCL low timeout. The SMBus is disabled 
// and re-enabled if a timeout occurs.
//
//----------------------------------------------------------------------------------------------------------
void Timer3_ISR (void) interrupt 14
{
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
   SMB_BUSY = 0;                       // Free bus
}

//----------------------------------------------------------------------------------------------------------
// Function Name:    SMBus_ISR()
//----------------------------------------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//
// SMBus ISR state machine
// - Master only implementation - no slave or arbitration states defined
// - All incoming data is written starting at the global pointer <pSMB_DATA_IN>
// - All outgoing data is read from the global pointer <pSMB_DATA_OUT>
//----------------------------------------------------------------------------------------------------------
void SMBus_ISR (void) interrupt 7 
{
   bit FAIL = 0;                       // Used by the ISR to flag failed transfers
   static char i = 0;                      // Used by ISR to count number data bytes sent/received
   static bit SEND_START = 0;          // Send a start
         
   switch (SMB0CN & 0xF0)              // Status vector
   {
      // Master Transmitter/Receiver: START condition transmitted.
      case SMB_MTSTA:
         SMB0DAT = TargetAddr;         // Load address of the target slave
         SMB0DAT &= 0xFE;              // Clear the LSB of the address for the
                                       // R/W bit
         SMB0DAT |= SMB_RW;            // Load R/W bit
         STA = 0;                      // Manually clear START bit
         i = 0;                        // Reset data byte counter
         break;

      // Master Transmitter: Data byte (or Slave Address) transmitted
      case SMB_MTDB:				   // Slave Address or Data Byte
      {    
         if (ACK)                      // Acknowledged?
         {                             
            if (SEND_START)
            {
               STA = 1;
               SEND_START = 0;
               break;
            }
            
            if(SendAddr_N == 1)     // Are we sending the destination address?
            {
               SendAddr_N = 0;   	// Clear flag
               SMB0DAT = BYTEADDR_Lo;    	// Send destination address low byte

               if (SMB_RANDOMREAD)
               {
                  SEND_START = 1;      	// Send a START after the next ACK cycle
                  if (1 == SMB_REPEATSTART) {
                      SMB_REPEATSTART = 0;
                      break;              
                  } 
                  SMB_RW = READ; // the next ISR cycle will be the reading cycle
               }
               break;   // now the xmit buffer is stuffed that will cause Trasmit interrupt
            }
            else
            if (SendAddr_N == 2)    // requested to send 16-bit address
            {    
               SendAddr_N = 1;   	// one more for address low byte
               SMB0DAT = BYTEADDR_Hi;  // Send destination address high byte
               break;   // now the xmit buffer is stuffed that will cause Trasmit interrupt     
            }
            
            if (SMB_RW==WRITE)         		// Is this transfer a WRITE?
            {
               if (i < SMB_DATA_LEN)   		// Is there data to send?
               {
                  SMB0DAT = *pSMB_DATA_OUT;	// send data byte
                  pSMB_DATA_OUT++;		 	// increment data out pointer
                  i++;	 					// increment number of bytes sent
               }
               else
               {
                 STO = 1;             		// Set STO to terminate transfer
                 SMB_BUSY = 0;        		// Clear software busy flag
                 SMB_DATA_LEN = 0;
               }
            }
            else 
            {
               ;// If this transfer is a READ, then take no action. 
            }                    
         }
         else                          // If slave NACK,
         {
            if (TargetAddr == LTC3220_ADDR1 || TargetAddr == LTC3220_ADDR2)    // the driver slave address 
            {
               MELEXIS_PWR = 1;  // Turn off the Melexis power
               LED_NACK_Detected = TRUE;
               MELEXIS_PWR = 0;  // Turn on the Melexis power
            }
            FAIL = 1;               // Indicate failed transfer and handle at end of ISR                  
         }
      }
      break;    // end of handling previous SMB_MTDB

      // Master Receiver: byte received
      case SMB_MRDB:
         if ( i < SMB_DATA_LEN )       // Is there any data remaining?
         {
            *pSMB_DATA_IN = SMB0DAT;   // Store received byte
            pSMB_DATA_IN++;            // Increment data in pointer
            i++;                       // Increment number of bytes received
            ACK = 1;                   // Set ACK bit (may be cleared later in the code)
         }

         if (i == SMB_DATA_LEN)        // This is the last byte
         {
            SMB_BUSY = 0;              // Free SMBus interface
            ACK = 0;                   // Send NACK to indicate last byte of transfer
            SMB_DATA_LEN = 0;
            STO = 1;                   // Send STOP to terminate transfer
            i = 0;
         }
         break;
	  // Transfer failed ...
      default:
         FAIL = 1;                     // Indicate failed transfer and handle at end of ISR
         break;		 
   }
   // If Transfer failed ...
   if (FAIL)                           
   {
      SMB0CF &= ~0x80;                 // Reset communication
      SMB0CF |= 0x80;
      STA = 0;
      STO = 0;
      ACK = 0;
      SEND_START = 0;   // drop the signal
      SMB_DATA_LEN = 0;
      i = 0;
      SMB_BUSY = 0;                    // Free SMBus
      FAIL = 0;
   }
   SI = 0;                             // Clear interrupt flag
}   // SMBus_ISR()

//--------------------------------------------------------------------------------------
// SMB_Poke()
// Description:     
// writes the value in <dat> to location <addr> in the device
// then polls the device until the write is complete.
// Return Value : 0 or 1 byte sent
// Parameters   :
//   1) unsigned char addr - address to write in the device
//   2) unsigned char dat - data to write to the address <addr> in the device
//---------------------------------------------------------------------------------------
int SMB_Poke(unsigned char addr, unsigned char dat, unsigned char target)
{	 
    unsigned int countdown = 500;

   if ( (target == LTC3220_ADDR1 || target == LTC3220_ADDR2 ) && TRUE == LED_NACK_Detected)
      return   -1;
	
    while ( SMB_BUSY )                 // Wait for SMBus to be free.
	{
        if ( --countdown == 0 )
        {
            SMB_BUSY = 0;
            return 0;  // no byte sent
        }	
   }	 
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TargetAddr = target;              	   // Set target slave address
   SMB_RW = WRITE;                     // Mark next transfer as a write
   SendAddr_N = 1;               // Send destination Address after Slave Address
   SMB_RANDOMREAD = 0;                 // Do not send a START signal after
                                       // the destination address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling (The ISR
                                       // will automatically restart the
                                       // transfer if the slave does not
                                       // acknoledge its address.
   // Specify the Outgoing Data
   BYTEADDR_Lo = addr;                 // address in the EEPROM's internal memory space

   SMB_SINGLEBYTE_OUT = dat;           // Store <dat> (local variable) in a
                                       // global variable so the ISR can read
                                       // it after this function exits
   // The outgoing data pointer points to the <dat> variable
   pSMB_DATA_OUT = &SMB_SINGLEBYTE_OUT;
   SMB_DATA_LEN = 1;                   // Specify to ISR that the next transfer
                                       // will contain one data byte
   // Initiate SMBus Transfer
   STA = 1;
   return 1;
}   // SMB_Poke()

//--------------------------------------------------------------------------------------
// SMB_WPoke()
// Description: sequential write block without repeat start    
// writes the value in <dat> to location <16-bit addr> in the device
// then polls the device until the write is complete.
// Return Value : 0 or 1 byte sent
// Parameters   :
//   1) unsigned char addrH, addrL - word address to write in the device
//   2) unsigned char dat - data to write to the address <addr> in the device
//---------------------------------------------------------------------------------------
int  SMB_WPoke(unsigned char addrH, unsigned char addrL, 
               unsigned char* pInBuf, unsigned int len, unsigned char target)
{
    unsigned int countdown = 10000;
	
    while ( SMB_BUSY )                 // Wait for SMBus to be free.
	{
        if ( --countdown == 0 )
        {
            SMB_BUSY = 0;
            return 0;  // no byte sent
        }	
    }	 
    SMB_BUSY = 1;               // Claim SMBus (set to busy)
    SMB_REPEATSTART = 0;        // not use repeat-start
    TargetAddr = target;        // Set target slave address
    SMB_RW = WRITE;             // Mark next transfer as a write
    SendAddr_N = 2;             // send 16-bit destination Address after Slave Address
    SMB_RANDOMREAD = 0;         // Do not send a START after the destination address                                  
    //SMB_ACKPOLL = 1;            // Enable Ack. Polling (The ISR will automatically
                                // restart the transfer if the slave does not acknowledge
    BYTEADDR_Hi = addrH;        // address in the EEPROM's internal memory space
    BYTEADDR_Lo = addrL;        // address in the EEPROM's internal memory space
    pSMB_DATA_OUT = pInBuf;     // Store the pointer
    SMB_DATA_LEN = len;         // Specify to ISR that the next transfer N bytes

    STA = 1;                    // Initiate SMBus Transfer
    return len;    
}

//--------------------------------------------------------------------------------------
// SMB_WPeek()
// Description: read mulitple bytes without repeat-start    
// read the value into <dat> from location <16-bit addr> in the device
// then polls the device until the write is complete. support random-read
// Return Value : 0 or byte length
// Parameters   :
//   1) unsigned char addrH, addrL - word address to write in the device
//   2) pointer to buffer to hold data peeked from address <addr> in the device
//---------------------------------------------------------------------------------------
int  SMB_WPeek(unsigned char addrH, unsigned char addrL, 
               unsigned char* dest_addr, unsigned char len, unsigned char target)
{
    unsigned int countdown = 10000;
	
    while ( SMB_BUSY )                 // Wait for SMBus to be free.
	{
        if ( --countdown == 0 )
        {
            SMB_BUSY = 0;
            return 0;  // bus is still busy
        }	
   }	 
   SMB_BUSY = 1;            // Claim SMBus (set to busy)
   TargetAddr = target;     // Set target slave address
   SMB_RW = WRITE;          // A random read starts as write then changes to read after repeated 
                            // start sent. The ISR handles this switchover if <SMB_RANDOMREAD> bit is set.
   SendAddr_N = 2;          // send 16-bit destination Address after Slave Address
   SMB_RANDOMREAD = 1;      // send a START after the destination address                                  
   SMB_ACKPOLL = 1;         // Enable Acknowledge Polling (The ISR will automatically
                            // restart the transfer if the slave does not acknoledge                            
   BYTEADDR_Hi = addrH;     // address in the EEPROM's internal memory space
   BYTEADDR_Lo = addrL;     // address in the EEPROM's internal memory space
   pSMB_DATA_IN=(unsigned char*) dest_addr;	// Set the the incoming data pointer
   SMB_DATA_LEN = len;      // Specify to ISR that next transfer will contain <len> data bytes
                            // will contain one data byte
   STA = 1;                 // Initiate SMBus Transfer
   countdown = 10000;
   while ( SMB_BUSY )       // Wait for SMBus to be free.
   {
     if ( --countdown == 0 )
     {
         SMB_BUSY = 0;
        return 0;
     }	
   }
   return len;    
}

//-----------------------------------------------------------------------------
// Function Name:    SMB_ReadArray()
//-----------------------------------------------------------------------------
// Read an array of characters from a SMBus device.
//-----------------------------------------------------------------------------
void SMB_ReadArray (unsigned char* dest_addr, unsigned char src_addr, 
                    unsigned char len, unsigned char target) 
{
   unsigned int  countdown = 10000;
   while ( SMB_BUSY )       // Wait for SMBus to be free.
   {
     if ( --countdown == 0 )
     {
         SMB_BUSY = 0;
        return;  
     }	
   }	 
   SMB_BUSY = 1;            // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TargetAddr = target;         // Set target slave address
   SMB_RW = WRITE;          // A random read starts as write then changes to read after repeated start sent.
                            // The ISR handles this switchover if <SMB_RANDOMREAD> bit is set.

   SendAddr_N = 1;    // Send destination Address after Slave Address
   SMB_RANDOMREAD = 1;      // Send a START after the word address
   SMB_ACKPOLL = 1;         // Enable Acknowledge Polling

   BYTEADDR_Hi = 0;         // address in the EEPROM's internal memory space
   BYTEADDR_Lo = src_addr;  // address in the EEPROM's internal memory space

   pSMB_DATA_IN=(unsigned char*) dest_addr;	// Set the the incoming data pointer
   SMB_DATA_LEN = len;      // Specify to ISR that next transfer will contain <len> data bytes
   STA = 1;					// Initiate SMBus Transfer

   countdown = 10000;
   while ( SMB_BUSY )       // Wait for SMBus to be free.
   {
     if ( --countdown == 0 )
     {
         SMB_BUSY = 0;
        return;
     }	
   }
}

//-----------------------------------------------------------------------------
// Function Name:   DEVICE_ThreeByteWrite ()
//
// Description:     
//
// Return Value : None
// Parameters   :
//   1) unsigned char addr - address to write in the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) unsigned char datLo - data to write to the low byte of address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
//   3) unsigned char datHi - data to write to the high byte of address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
//	 4) unsigned char target - slave address of the target device on the SMBus.
//
// This function writes the values in <datLo> and <dataHi> to location <addr> in the EEPROM.
// It sends the calculated PEC value so that the slave device can check that the sent values
// are correct.  It then polls the EEPROM until the write is complete.
//
//-----------------------------------------------------------------------------
void DEVICE_ThreeByteWrite(unsigned char addr, unsigned char dataLo, unsigned char dataHi, unsigned char target)
{
     unsigned char Pecreg;					//Calculated PEC byte storage
     unsigned char arr[6];					//Buffer for the transmitted bytes
     bits16 countdown = 10000;
    
     arr[5]=0;
     arr[4]=target;			//SLA;
     arr[3]=addr;			//cmdW;	  0x24
     arr[2]=dataLo;			// 0xFF
     arr[1]=dataHi;			// 0xFF
     arr[0]=0;

	 Melexis_Payload[0] = arr[2];
	 Melexis_Payload[1] = arr[1];	  // ERROR - sets emissivity to 0x0024 instead of 0xFFFF

     Pecreg=MEL_CalculatePEC(arr);	  // PEC_calculation changes arr values

	 Melexis_Payload[2] = Pecreg;

     while ( SMB_BUSY )       // Wait for SMBus to be free.
     {
        if ( --countdown == 0 )
          {
             SMB_BUSY = 0;
             return;  
          }	
    }	  
   SMB_BUSY = 1;            // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TargetAddr = target;         // Set target slave address
   SMB_RW = WRITE;          // Mark next transfer as a write
   SendAddr_N = 1;    // Send destination Address after Slave Address
   SMB_RANDOMREAD = 0;      // Do not send a START signal after
                            // the word address
   SMB_ACKPOLL = 1;         // Enable Acknowledge Polling (The ISR
                            // will automatically restart the transfer
                            // if the slave does not acknoledge its address.
   // Specify the Outgoing Data
   BYTEADDR_Lo = addr;      // Set the target address in the internal memory space
   pSMB_DATA_OUT = Melexis_Payload;
   SMB_DATA_LEN = 3;        // so that ISR in the next transfer know number of bytew
   
   STA = 1; // Initiate SMBus Transfer
}

//-----------------------------------------------------------------------------
// function: SMB_seqWrBlk()
// description: sequential write block without repeat start
// parameter: memory or register of the target, 
//            pointer to input buffer, buffer length, slave address
// return: 0 if bus is busy and no transaction took place, otherwise, the buffer length
//-----------------------------------------------------------------------------
int SMB_seqWrBlk(unsigned char addr, unsigned char* pDataBuf, int len, unsigned char target)
{
    bits16 countdown = 10000;
    while ( SMB_BUSY )       // Wait for SMBus to be free.
    {
        if ( --countdown == 0 )
        {
            SMB_BUSY = 0;
            return 0;  
        }	
    }	  
    SMB_BUSY = 1;            // Claim SMBus (set to busy)

    // Set SMBus ISR parameters
    TargetAddr = target;     // Set target slave address
    SMB_RW = WRITE;          // Mark next transfer as a write
    SendAddr_N = 1;          // Send destination Address after Slave Address
    SMB_RANDOMREAD = 0;      // Do not send repeat START signal
    SMB_ACKPOLL = 1;         // Enable Acknowledge Polling (The ISR will automatically
                             // restart the transfer if the slave does not acknoledge

    // Specify the Outgoing Data
    BYTEADDR_Lo = addr;      // Set the target address in the internal memory space
    pSMB_DATA_OUT = pDataBuf;
    SMB_DATA_LEN = len;      // so that ISR in the next transfer know number of bytes
   
    STA = 1; // Initiate SMBus Transfer
    return len;
}

// -----------------------------------------------------------------------------
// Function Name:   SMB_WrByte ()
// Description:     
// This function writes the payload to the device. location not needed
// then polls the device until the write is complete.
// Return Value : 0 or 1 byte sent
// Parameters   :
//   1) unsigned char dat - data to write to the address <addr> in the device
//   2) slave address
//   3) repeat start specifier
//-----------------------------------------------------------------------------
int SMB_WrByte(unsigned char dat, 
               unsigned char SlaveAddr, unsigned char repeatstart)
{
	unsigned int  countdown = 10000;

	while ( SMB_BUSY )                   // Wait for SMBus to be free.
	{
		if (--countdown == 0)
		{
			SMB_BUSY = 0;
			return 0;	// time out
		}
	}
	SMB_BUSY = 1;           // Claim SMBus (set to busy)

    // Set SMBus ISR parameters
    TargetAddr = SlaveAddr;         // Set target slave address
	SMB_RW = WRITE;             // Mark next transfer as a write
    SendAddr_N = 0;   	// no need to provide memory address
	SMB_RANDOMREAD = repeatstart & 1;
	SMB_ACKPOLL = 1;        // Enable Ack. Polling. The ISR will automatically
                            // restart the transfer if the slave does not acknoledge	

    SMB_SINGLEBYTE_OUT = dat;    // Store <dat> (local variable) in a global variable
                                // so the ISR can read it after this function exits
                                   
    // The outgoing data pointer points to the <dat> variable
    pSMB_DATA_OUT = &SMB_SINGLEBYTE_OUT;
    SMB_DATA_LEN = 1;           // Specify to ISR that the next transfer 

    STA = 1;    // Initiate SMBus Transfer
    
    return 1;
}

//----------------------------------------------------------------------------
int SMB_WrByte_1W(unsigned char cmd, unsigned char dat, unsigned char SlaveAddr)
{	 
	 unsigned int  countdown = 400;
	
   while ( SMB_BUSY )                   // Wait for SMBus to be free.
   {
     if ( --countdown == 0 )
     {
        SMB_BUSY = 0;
        return 0;  
     }	
   }	 
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TargetAddr = SlaveAddr;        // Set target slave address
   SMB_RW = WRITE;            // Mark next transfer as a write
   SendAddr_N = 1;      // Send destination Address after Slave Address
   SMB_RANDOMREAD = 0;        // Do not send a repeat START  
   SMB_ACKPOLL = 0;           // 1: Enable Acknowledge Polling (The ISR will automatically 
                              // restart the transfer if the slave does not acknoledge
   // Specify the payload
   BYTEADDR_Lo = cmd;         // overload the variable to use for cmd paramter

   SMB_SINGLEBYTE_OUT = dat;  // Store <dat> (local variable) in a global variable
                              // so the ISR can read it after this function exits

   // The outgoing data pointer points to the <dat> variable
   pSMB_DATA_OUT = &SMB_SINGLEBYTE_OUT;
   SMB_DATA_LEN = 1;          // Specify to ISR that the next transfer
                              // will contain one data byte
   // Initiate SMBus Transfer
   STA = 1;
   
   return 1;
}

//-----------------------------------------------------------------------------
int SMB_RdByte_1W(unsigned char *dest_addr, unsigned char SlaveAddr)
{
	unsigned int  countdown = 4000;
/*
	while ( SMB_BUSY )                   // Wait for SMBus to be free.
	{
		if (--countdown == 0)
		{
			SMB_BUSY = 0;
			return 0;	// time out
		}
	}
*/
   while ( (SMB0CF & 0x20) >> 5) // Wait for SMBus to be free.
	{
		if (--countdown == 0)
		{
         STO = 0;
         ACK = 0;
         SMB_DATA_LEN = 0;
         SMB_BUSY = 0;                    // Free SMBus
         return 0;	// time out
		}
	}
   SMB_BUSY = 1;           // Claim SMBus (set to busy)
    
    pSMB_DATA_IN = (unsigned char*)dest_addr;	// Set the the incoming data pointer
    TargetAddr = SlaveAddr;
    SendAddr_N = 0;    // don't need to send destination after slave address
	SMB_DATA_LEN = 1;    // Specify to ISR that next transfer will contain <len> data bytes
	SMB_RW = READ;   // A random read starts as write then changes to read after repeated start sent.
    SMB_RANDOMREAD = 0;
	// The ISR handles this switch-over if <SMB_RANDOMREAD> bit is set.
	SMB_ACKPOLL = 0;           // 1: Enable Acknowledge Polling
    STA = 1;						// Initiate SMBus Transfer

   countdown = 4000;
   while ( (SMB0CF & 0x20) >> 5) // Wait for SMBus to be free.
	{
		if (--countdown == 0)
		{
         STO = 0;
         ACK = 0;
         SMB_DATA_LEN = 0;
         SMB_BUSY = 0;                    // Free SMBus
		}
	}
   
    return 1;
}


//----------------------------------------------------------------------------------------------------------
// End Of File
//----------------------------------------------------------------------------------------------------------
