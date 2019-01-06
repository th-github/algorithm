// th_algo.h

#ifndef TH_ALGO_H
#define TH_ALGO_H


/****** Fibonacci sequence practices ******/
// iteration method
unsigned long long fib(unsigned int n, 
    unsigned long long *p, unsigned long long *c);
// recursion method
unsigned long long fib_rcs(unsigned int n);
// 
unsigned long long fib_lt(unsigned int n);

extern unsigned int call_counts;

#endif

