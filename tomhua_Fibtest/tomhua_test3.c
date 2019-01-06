
// tomhua_test3.c
// this version compares the performance of two implementations
// execution time is taking with snap shots of system ticks 

// Fibonacci number at 100 is 354,224,848,179,261,915,075

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "th_algo.h"

unsigned long long fib(unsigned int n, 
    unsigned long long *p, unsigned long long *c);
unsigned long long fib_rcs(unsigned int n);


void main(int argc, char *argv[])
{
    unsigned int n = 0;
    unsigned long long f = 0, p = 0, c = 1;

    
    printf("------------ tomhua_test3.c  --------------\n");
    printf("Fibonacci number practices ----------------\n");
        
    
    if (argc < 2) {
        printf(" Please Enter [0..100]: ");
        do {
            scanf("%d", &n);
        
        if (n < 0 || n > 100)
            printf(" number out of range, try again\n : ");   

        } while (n < 0 || n > 100);
    }
    else
        n = strtol(argv[1], NULL, 10);
    
    if (n > 100) {
        printf(" The number %d entered is out of range. Exit. \n", n);
        printf("--------------------------------------------\n");
        return;
    }
    
    printf(" (Note, printf hanldes up to:  %lu))\n\n", ULONG_MAX);
    
    printf(" Linear iteration -- \n");
    clock_t start = clock();
    f = fib(n, &p, &c);   
    clock_t finish = clock();
    
    printf(" process time : %4.3f seconds, (%lu time_units)\n", 
            (float)((finish-start)/CLOCKS_PER_SEC), (unsigned long)(finish-start));

    if (n < 94) {
        printf(" The %dth Fibonacci number is: %llu\n", n, f);
        if (n > 1)
            printf( " It's the sum of %dth: %llu and \n\t\t %dth: %llu\n", 
                (n-2), p, (n-1), c);
    }
    if (n > 93)
    {
        f = p+c;
        // The printf in standard C library won't handle bigger number
        printf(" The %dth Fibonacci number in Hex is:\n\t %llX, sum of:\n", 
                n, f);
        printf(" %dth: \t %llX\n", (n-2), p);
        printf(" %dth: \t %llX\n", (n-1), c);
     
    }  
    printf("\n");
    
    printf(" Recursion. process needs more access of stack and memory: \n");

    if (n >= 45) {
        n = 45;
        printf(" For testing, any bigger entry is clamped to be 45\n");
        printf(" Recursive method for this calculation takes a while ...\n");
        printf(" as it push/pop stacks and access memory more often.\n");
    }
    start = clock();
    f = fib_rcs(n);  
    finish = clock();

    printf(" process time : %4.3f seconds, (%lu time_units)\n", 
            (float)((finish-start)/CLOCKS_PER_SEC), (unsigned long)(finish-start));
                    
    printf(" The %dth Fibonacci number is: %llu\n", n, f);

    printf(" number of calls to fib(%u): %u\n", n, call_counts);
        
    if(n < 46) { 
        printf("\n By lookup-table, direct memory access, no calculation.\n");
        f = fib_lt(n);  
        printf(" The %dth Fibonacci number is: %llu\n", n, f);    
    }
        
    printf("--------------------------------------------\n");
    
}




