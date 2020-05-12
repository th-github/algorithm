/** Author: Tom Hua
padandpack.c
	struct type data padding and packing, in 32-bit machine
**/

#include <stdio.h>

void fp(void);

int main(void)
{  
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
                        
    printf("sizeof struct st1: %d\n", (int)sizeof(struct type_st1)); 
    printf("sizeof struct st2: %d\n", (int)sizeof(struct type_st2));            
    printf("sizeof struct st3: %d\n", (int)sizeof(struct type_st3));
    printf("sizeof struct st4: %d\n", (int)sizeof(struct type_st4));
    printf("sizeof struct st5: %d\n", (int)sizeof(struct type_st5));
    printf("sizeof struct st6: %d\n", (int)sizeof(struct type_st6));

    printf("sizeof void*: %d\n", (int)sizeof(void*));
    printf("sizeof struct ft: %d\n", (int)sizeof(struct type_ft));
                                  
    printf("\n"); 
	return 0;
}


