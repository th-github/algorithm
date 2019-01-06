
// th_algo_Fibonacci.c
// Fibonacci number at 100 is 354,224,848,179,261,915,075

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "th_algo.h"

unsigned int call_counts = 0;

unsigned long long fib_table[46] =
{
    0, 1, 1, 2, 3,                     5, 8, 13, 21, 34, 
    //10th: 
    55, 89, 144, 233, 377,             610, 987, 1597, 2584, 4181,
    //20th, ...
    6765, 10946, 17711, 28657, 46368,  75025, 121393, 196418, 317811, 514229,
    //30th, ...
    832040,  1346269,  2178309,  3524578,  5702887,
    //35th, ...
    9227465, 14930352, 24157817, 39088169, 63245986,
    // 40th, ...
    102334155, 165580141,267914296,433494437,701408733,
    // 45th:
    1134903170
};

unsigned long long fib(unsigned int n,
    unsigned long long *prev, unsigned long long *current)
{
    unsigned int i = 0;
    unsigned long long f = 0;
    unsigned long long p = 0, c = 1;

    if (n < 1) {
        *prev =0;
        *current = 1;
        return 0;
    }
    if (n <= 2) {
        *prev = 0;
        *current = 1;
        return 1; 
    }
                       
    for(i = 2; i <= n; i++) {
        { 
            *prev = p;  
            *current = c;
            f = p + c; 
            p = c;
            c = f;
        }

    }
    
    return f;
}

unsigned long long fib_rcs(unsigned int n)
{
    call_counts++;
    if (n < 1)
        return 0;
    if (n <= 2)
        return 1; 

    return ( fib_rcs((n - 1)) + fib_rcs((n - 2)) );
}

unsigned long long fib_lt(unsigned int n)
{
    return fib_table[n];
}


