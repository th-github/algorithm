// Tom Hua. data structure practice

#include <iostream>
#include <bitset>
//#include <bits/stdc++.h>

using namespace std;

int countbits(unsigned int data);   // count bit ones
int b2d(int data);  // bin to dec
int testAlternatePattern(unsigned int data);
void swaptwobits(int* data, int b1, int b2);
void test_swaptwobits(int* data, int b1, int b2);
int isNumOposit(int x, int y);
void test_NumOposit(int n1, int n2);
int togglebits(int data);
int rotatebits(int data);
void test_findlsb_1bit(unsigned int data);

void findlsb_0bit(int data);
int changeselectbits_16(int data, int v, int u, int l, bool toggle);
void test_clearbits(unsigned int data, int u, int l);
void test_setbits(unsigned int data, int u, int l);
void clr_lsb(unsigned int* data, int ith);
void set_msb(unsigned int* data, int ith);
unsigned short rotatebits(unsigned short);
void dec2bin(unsigned int, unsigned int* bits);

int main(void)
{
    unsigned int data = 0x11A;
    unsigned long n = 0L;
    int val = 0, ret = 0;
    int counts = 0;
	static unsigned int bits[32];

	dec2bin(data, bits);
	cout << endl;

    counts = countbits(data);
    cout << "bit counts in data: " << counts << endl << endl;

    val = b2d(data);
    cout << " binary: " << bitset<16>(data) << " to integer: " << val << endl << endl;

    cout << "msb_1bit in: " << bitset<16>(data) << " is at: " 
         << counts << "th position" << endl << endl;

    val = testAlternatePattern(data);
    cout << "test alternate pattern of: " << val << endl << endl;
    data = 0xaaaa;
    val = testAlternatePattern(data);
    cout << "test alternate pattern of: " << val << endl << endl;

    val = 28;
    test_swaptwobits(&val, 2,6);
    cout << endl;
    val = 43;
    test_swaptwobits(&val, 2,6);
    cout << endl;
    val = 7;
    test_swaptwobits(&val, 2,6);
    cout << endl;
        
    test_NumOposit(10, -10);
    cout << endl;
    test_NumOposit(5, 9);
    cout << endl;
    test_NumOposit(7, 8);
    cout << endl;
	        
    test_findlsb_1bit(1);
    test_findlsb_1bit(255);
    test_findlsb_1bit(128);
    test_findlsb_1bit(29);
    test_findlsb_1bit(28);
    test_findlsb_1bit(26);
    test_findlsb_1bit(0);            
    cout << endl;
    
	findlsb_0bit(26);
    findlsb_0bit(29);
    findlsb_0bit(27);
    findlsb_0bit(128);
    findlsb_0bit(255);
    cout << endl;
	
    val = 29;
    cout << "val: " << bitset<8>(val) << endl;
    val = togglebits(val);
    cout << "toggled: " << bitset<8>(val) << endl;
    cout << endl;
    
    val = 29;
    cout << "val: " << bitset<8>(val) << endl;
    val = rotatebits(val);
    cout << "rotatebits: " << bitset<8>(val) << endl;
    cout << endl;
        
    cout << "change bits between[5,2] " << bitset<16>(val) << endl;
    data = changeselectbits_16(val, 3, 5,2, false); // toggle: false
    cout << "bits replaced " << bitset<16>(data) << endl;
    cout << endl;

    cout << "change bits between[5,2] " << bitset<16>(val) << endl;
    data = changeselectbits_16(val, 12, 5,2, false); // toggle: false
    cout << "bits replaced " << bitset<16>(data) << endl;
    cout << endl;
        
    n = 0xcafe;
    cout << "toggle bits between[10,3] " << bitset<16>(n) << endl;
    data = changeselectbits_16(n, 0, 10,3, true); // toggle: true
    cout <<  "bits toggled " << bitset<16>(data) << endl;
    cout << endl;

    cout << "toggle bits between[15,0] " << bitset<16>(n) << endl;
    data = changeselectbits_16(n, 0, 15,0, true); // toggle: true
    cout << "bits toggled " << bitset<16>(data) << endl;
    cout << endl;
                        
    test_clearbits(0xcafe, 10,3);
    cout << endl;
    test_clearbits(0xcafe, 10,0);
    cout << endl;
    test_setbits(0xcafe, 10,3);
    cout << endl;
    test_setbits(0xcafe, 10,0);
    cout << endl; 
    
    bitset<8*sizeof(short)> b;
    b = 0xcafe;
    cout << "b: " << b << endl;
    cout << "b: " << b[15] << " " << b[0] << endl; 
    
    data = 0x8F79;
    n = 6;
    cout << "data: " << bitset<16>(data) << endl;
    cout << "clear all lsb from bit " << n << endl;    
    clr_lsb(&data, n);
    cout << "data: " << bitset<16>(data) << endl; 
    cout << endl;
    
    data = 0x020;
    set_msb(&data, 6);
    data = 0x10;
    set_msb(&data, 1);
    cout << endl; 
     
    cout << "-----------" << endl; 
    return 0;
}

void dec2bin(unsigned int d, unsigned int* bits)
{
	int i = 31;
	while (i--)
		bits[i] = (d >> i) & 1;

	i = 31;
	do {
		cout << bits[i];
	} while (i--);
	cout << endl;
	
}

int b2d(int data)
{
    int v = data & 1;


    //if (data < 256) 
    {
        v += (1 & (data >> 1))*2;
        v += (1 & (data >> 2))*4;
        v += (1 & (data >> 3))*8;   

        v += (1 & (data >> 4))*16;
        v += (1 & (data >> 5))*32;
        v += (1 & (data >> 6))*64;
        v += (1 & (data >> 7))*128;
    }    
    //if (data > 255) 
    {
        v += (1 & (data >> 8))*256;
        v += (1 & (data >> 9))*512;
        v += (1 & (data >> 10))*1024;   

        v += (1 & (data >> 11))*2048;
        v += (1 & (data >> 12))*4096;
        v += (1 & (data >> 13))*8292;
        v = !v;
    } 

    return v;
}

int testAlternatePattern(unsigned int data)
{
    unsigned int ret = 0;

    ret = (data ^ (data >> 1));

    if (ret == 0xFFFF)
        return 1;
    return 0;
}

void test_swaptwobits(int* data, int b1, int b2)
{
	int v1, v2;
	v1 = *data & (1 << b1);
	v2 = *data & (1 << b2);
	cout << "swap two bits [" << b1 << "," << b2 << "] "<< endl;
	cout << " in: " << bitset<16>(*data) << endl;
	if (v1 != v2)
    {
        *data ^= (1 << b1);
        *data ^= (1 << b2);
    }
    
    cout << "new: " << bitset<16>(*data) << endl;     
}

int isNumOposit(int x, int y)
{
    return ((x^y) < 0);
}

void test_NumOposit(int n1, int n2)
{    
    cout << n1 << "," << n2 << " is oposit? " << ((n1^n2) < 0) << endl;
}

void test_findlsb_1bit(unsigned int data)
{
    int i = 1, v = data;
    if (v == 0) {
        cout << "there is no lsb_1bit in " << bitset<8>(data) << endl;
        return;
    }
    
    while(v) {
        if (v & 1) {            
            cout << "lsb_1bit " << bitset<8>(data) << " @ " << (i-1) << endl;
            break;    
        }
        v = v >> 1;  
        i++; 
   }               
}

void findlsb_0bit(int data)
{
    int i = 1, v = data;
    while(v) {
        if (v & 1) {  
            v = v >> 1;  
            i++;  
        }         
        else {
			cout << "lsb_0bit " << bitset<8>(data) << " @ " << (i-1) << endl;
            break; 
		}   
   }
   if (i > 7)
	   cout << "there is no lsb_0bit in " << bitset<8>(data) << endl;
}

int togglebits(int data)
{
    unsigned int len = 0;;
    unsigned int temp = data, mask = 0;
    
    while (temp)
    {
        if (temp & 1)   len++;
        temp = (temp >> 1);
    }
    mask = (1 << (len+1)) - 1;  // make a set-bit mask start from msb 1-bit
    return data ^ mask;  

}

int rotatebits(int data)
{
    int len = 0, m, i=0, j=0, result = 0;
    unsigned int temp = data;
    
    while (temp)
    {
        if (temp & 1)   len++;
        temp = (temp >> 1);
    }
    m = (int)(len/2);

    while (i<=m) {
        result |= ((data & (1<<(len-i))) >> (len-i-j));
        i++;
        j++;
    }
    i = 0;
    j = 0;
    while (i<=m) {
        result |= ((data & (1<<i)) << (len-i-j));
        i++;
        j++;
    }
          
    return result;
}

int changeselectbits_16(int source, int v, int u, int l, bool toggle)
{
    int mask, temp = source;
    mask = (0xFFFF << (u+1)) | ((1 << l)-1);  // ie., ..1100011..
    cout << "mask: " << bitset<16>(mask) << "  ";
    if (toggle) {
        cout <<  "toggle bits" << endl;  
        return  source ^ (~mask);
    }
    cout << "replace " << v << " " << bitset<8>(source) << endl;
    temp &= mask;
    v = v << l;
    
    return temp | v;
}

void test_clearbits(unsigned int data, int u, int l)
{
    unsigned int mask = 0;
	unsigned int n = (short)(sizeof(unsigned int) * 8);

    if (u >= n || l < 0) {
		cout << "invalid range given" << endl;
		return;
	}

	mask = 0xFFFFFFFF >> (n - u);
	mask &= 0xFFFFFFFF << l;
	mask = ~mask;
    cout << "mask clr: " << bitset<32>(mask) << endl;
        
    cout << "old data: " << bitset<32>(data) << endl;
    cout << "new data: " << bitset<32>(data & mask) << endl;    
}

void test_setbits(unsigned int data, int u, int l)
{
    unsigned int mask = 0;
	unsigned short n = (short)(sizeof(unsigned int) * 8);

    if (u >= n || l < 0) {
		cout << "invalid range given" << endl;
		return;
	}

	mask = 0xFFFFFFFF >> (n - u);
	mask &= 0xFFFFFFFF << l;	
    cout << "mask set: " << bitset<32>(mask) << endl;
        
    cout << "old data: " << bitset<32>(data) << endl;
    cout << "new data: " << bitset<32>(data | mask) << endl;    
}

void clr_lsb(unsigned int* data, int ith)
{
    unsigned int m = (~0 << (ith+1));   // ie., 0xFF...FFF0000
    *data &= m;
}

void set_msb(unsigned int* data, int ith)
{
    unsigned int m = (~0 << ith);  // ie., 0x...FFF0000...
    cout << "set all msb " << bitset<8>(*data) << " from bit " << ith << endl; 
    *data |= m;
    cout << "new result: " << bitset<8>(*data) << endl;
}

unsigned short rotatebits(unsigned short v)
{
	unsigned short n = 0;

	n |= (0x80 & v) >> 7;
	n |= (0x40 & v) >> 5;
	n |= (0x20 & v) >> 3;
	n |= (0x10 & v) >> 1;
	n |= (0x08 & v) << 1;
	n |= (0x04 & v) << 3;
	n |= (0x02 & v) << 5;
	n |= (0x01 & v) << 7;

	return n;
}

int countbits(unsigned int data)
{
    int n = 0;
    
    while (data)
    {
        data &= (data-1);
        n++;
    }
    return n;
}


/*********** output **********
Admins-MacBook-Pro:algorithm-master admin$ ./a.out
00000000000000000000000100011010

bit counts in data: 4

 binary: 0000000100011010 to integer: 0

msb_1bit in: 0000000100011010 is at: 4th position

test alternate pattern of: 0

test alternate pattern of: 1

swap two bits [2,6] 
 in: 0000000000011100
new: 0000000001011000

swap two bits [2,6] 
 in: 0000000000101011
new: 0000000000101011

swap two bits [2,6] 
 in: 0000000000000111
new: 0000000001000011

10,-10 is oposit? 1

5,9 is oposit? 0

7,8 is oposit? 0

lsb_1bit 00000001 @ 0
lsb_1bit 11111111 @ 0
lsb_1bit 10000000 @ 7
lsb_1bit 00011101 @ 0
lsb_1bit 00011100 @ 2
lsb_1bit 00011010 @ 1
there is no lsb_1bit in 00000000

lsb_0bit 00011010 @ 0
lsb_0bit 00011101 @ 1
lsb_0bit 00011011 @ 2
lsb_0bit 10000000 @ 0
there is no lsb_0bit in 11111111

val: 00011101
toggled: 00000010

val: 00011101
rotatebits: 00010111

change bits between[5,2] 0000000000010111
mask: 1111111111000011  replace 3 00010111
bits replaced 0000000000001111

change bits between[5,2] 0000000000010111
mask: 1111111111000011  replace 12 00010111
bits replaced 0000000000110011

toggle bits between[10,3] 1100101011111110
mask: 1111100000000111  toggle bits
bits toggled 1100110100000110

toggle bits between[15,0] 1100101011111110
mask: 0000000000000000  toggle bits
bits toggled 0011010100000001

mask clr: 11111111111111111111110000000111
old data: 00000000000000001100101011111110
new data: 00000000000000001100100000000110

mask clr: 11111111111111111111110000000000
old data: 00000000000000001100101011111110
new data: 00000000000000001100100000000000

mask set: 00000000000000000000001111111000
old data: 00000000000000001100101011111110
new data: 00000000000000001100101111111110

mask set: 00000000000000000000001111111111
old data: 00000000000000001100101011111110
new data: 00000000000000001100101111111111

b: 1100101011111110
b: 1 0
data: 1000111101111001
clear all lsb from bit 6
data: 1000111100000000

set all msb 00100000 from bit 6
new result: 11100000
set all msb 00010000 from bit 1
new result: 11111110

-----------

*************/


