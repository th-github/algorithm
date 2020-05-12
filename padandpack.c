/** Author: Tom Hua
padandpack.c
	struct type data padding and packing, in 32-bit machine
**/

#include <stdio.h>

void fp(void);

int main(void)
{
    int v = 0;
    
    struct type_st1 {
        char c;     // 2 bytes,  padded bytes to align to type long
        long l;     // 8 bytes  full width
        short s;    // 2 bytes   padded bytes to align to type long    
    };
    struct type_st2 {
        char c;     // 2 bytes,  padded to type int 
        int  i;     // 4 bytes
        short s;    // 2 bytes   padded to type int     
    };
    struct type_st3 {   // declare in decreasing order to minimize padding
        double l;     // 8 bytes  full width
        short  s;     // 2 bytes    packed with the next shorter one
        char c;       // 2 bytes    packed with the former longer one
    };
    
    struct type_st4 {
        int  i;     // 4 bytes
        float l;    // 4 bytes  
        short s;    // 2 bytes   padded to type float
    };

    struct type_st5 {
        long l;     // 8 bytes  full width
        short s;    // 2 bytes   padded to type int
        int  i;     // 4 bytes   
    };

    struct type_st6 {   // declare in increasing order
        int  i;       // 4 bytes   padded to type long
        double l;     // 8 bytes  full width
    };
        
    struct type_ft {  
        void  (*fp)(void);       // 8 bytes, full width
        int i;  // 4 bytes
        char c; // padded to type int 
    };
                        
    struct type_st1   st1;
    v = sizeof(st1);
    printf("sizeof struct st1: %d\n", v); 
    
    struct type_st2   st2;
    v = sizeof(st2);
    printf("sizeof struct st2: %d\n", v);    
         
    struct type_st3   st3;
    v = sizeof(st3);
    printf("sizeof struct st3: %d\n", v);

    struct type_st4   st4;
    v = sizeof(st4);
    printf("sizeof struct st4: %d\n", v);

    struct type_st5   st5;
    v = sizeof(st5);
    printf("sizeof struct st5: %d\n", v);

    struct type_st6   st6;
    v = sizeof(st6);
    printf("sizeof struct st6: %d\n", v);

    struct type_ft   ft;
    v = sizeof(void*);
    printf("sizeof void*: %d\n", v);
    v = sizeof(ft);
    printf("sizeof struct ft: %d\n", v);
                                  
    printf("\n"); 
	return 0;
}


