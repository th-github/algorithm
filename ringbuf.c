/** Author: Tom Hua
	ringbuf.c
**/

#include <stdio.h>


#define BUFSIZE 8
static int nRear, nFront;
static int rb[BUFSIZE];
static int nSpaceAvail = BUFSIZE;
static int head;
void iWr(int d);
int iRd(int* d);

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
 
    for (int i = 0; i < 9; i++)
    {     
        space = iRd(&v);
        if (space > 0)
            printf("%d ", v);
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
    if ((nRear == nFront) && (nSpaceAvail < 1))   // buf is full
    {
        nRear = nFront; // allow over-write the current spot
        nFront++;       // thus, make the next previous one be the nfront 
        if (nFront > 7)
            nFront = 0;
        
        nSpaceAvail = 0;    // available space is zero
    }
    else
        nSpaceAvail--;
        
    rb[nRear++] = d;
    if (nRear > 7)
        nRear = 0;       
}


int iRd(int* d)
{
    
    if (8 == nSpaceAvail)
        return 0;
        
    *d = rb[nFront++];
    nSpaceAvail++;
    
    if (nFront > 7)
        nFront = 0;
        
    
   return nSpaceAvail;
}

