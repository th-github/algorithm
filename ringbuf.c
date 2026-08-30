/** Author: Tom Hua
	ringbuf.c
	FIFO circular overwritable, LIFO not overwritable
**/

#include <stdio.h>


#define BUFSIZE 8
static int nRear, nFront, nTop;
static int rb[BUFSIZE];
static int nSpaceAvail = BUFSIZE;
static int bottom = 1;

void iWr(int d);
int iRd(int* d);
int push(int d);
int pop(int* d);

int main(void)
{
    int v = 0, space = 0;
    
    iWr(34);
    iWr(12);
    iWr(20);
    iWr(56);
    
    iWr(3);
    iWr(64);
    iWr(65);
    iWr(7);  
    
    iWr(5);
    iWr(27);      

    space = iRd(&v);
    printf("data: %d ", v);
    printf("\n");       
         
    iWr(41);
    iWr(0);  
    iWr(14);
 
 	printf("\n");
 
    for (int i = 0; i <= BUFSIZE; i++)
    {     
        space = iRd(&v);
        if (space > 0)
            printf("%d ", v);
    }

	printf("\nLIFO push test: \n");
    for(int i = 0; i < BUFSIZE; i++)
	{
		if (push(i) >= BUFSIZE)
		{
			printf("stack full\n");
		}
	}
	for (int i = 0; i < BUFSIZE; i++)
	{
		printf(" %d", rb[i]);
	}
	printf("\n");
	
	push(8);
	printf(" %d", rb[0]);
	
	printf("\nLIFO pop test: \n");
	int ret = -1;
	int d = -1;
	for (int i = 0; i < 5; i++)
	{
		if ((bottom != 0) && (nTop >= 0))
		{
			ret = pop(&d);
			printf(" %d,", d);
		}
		else 
		{
			printf("\n LIFO empty\n");
		}
	}
	printf("\n");
	
	if ((bottom != 0) && (nTop >= 0))
	{
		ret = pop(&d);
		printf(" %d,", d);
	}
	else 
	{
		printf("\n LIFO empty\n");
	}

	if ((bottom != 0) && (nTop >= 0))
	{
		ret = pop(&d);
		printf(" %d,", d);
	}
	else 
	{
		printf("\n LIFO empty\n");
	}
			
	if ((bottom != 0) && (nTop >= 0))
	{
		ret = pop(&d);
		printf(" %d,", d);
	}
	else 
	{
		printf("\n LIFO empty\n");
	}
			                 
    printf("\n"); 
	return 0;
}

void iWr(int d)
{
    /* 
        nRear advances 0->7, wraps around to 0 ... on each WR op
        when no Wr only and Rd op, nRear reaches to same as nFront
        Rd op needs to over-write the same spot as the cureent nFront.
        Thus nFront needs to advance. In this condition the nRear < nFront.  
    */
    printf("wr: nSpaceAvail: %d, nFront: %d, Rear: %d\n", nSpaceAvail, nFront, nRear);    
    if ((nRear == nFront) && (nSpaceAvail < 1))   // buf is full
    {
        //nRear is a write pointer, nFront is a read pointer
        nFront++;       // thus,prepare the front pointer to the previous one
        if (nFront > (BUFSIZE-1))	// has reaqched beyond tyhe end of buffer
            nFront = 0;
        
        nSpaceAvail = 0;    // available space is zero
    }
    else
        nSpaceAvail--;
    

    rb[nRear++] = d;
    if (nRear > (BUFSIZE-1))	// has reaqched beyond tyhe end of buffer
        nRear = 0;
	printf(" nSpaceAvail: %d, nFront: %d, Rear: %d\n", nSpaceAvail, nFront, nRear);    	       
}


int iRd(int* d)
{
    
    if (8 == nSpaceAvail) // buffer is empty
    {
    	printf("\n buffer is empty\n");  
        return 0;
	}
    printf("rd: nSpaceAvail: %d, nFront: %d, Rear: %d\n", nSpaceAvail, nFront, nRear);     
    *d = rb[nFront++];
    nSpaceAvail++;
    
    if (nFront > (BUFSIZE-1))	// has reaqched beyond tyhe end of buffer
        nFront = 0;
    printf(" nSpaceAvail: %d, nFront: %d, Rear: %d\n", nSpaceAvail, nFront, nRear);       
    
   return nSpaceAvail;
}

int push(int d)
{
	if(nTop < BUFSIZE)
	{
		if (nTop < 0)
			nTop = 0;
		rb[nTop++] = d;
	}
	
	return nTop;
}

int pop(int* d)
{
	if (nTop >= BUFSIZE)
		nTop = BUFSIZE - 1;
	if (nTop >= 0)
	{
		*d =  rb[nTop--];
	}
	if (nTop < 0)
	{
		bottom = 0;
	}
	return (nTop + 1);
}
