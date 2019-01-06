
th_readme.txt for Fibonacci sequence programming practices
Author: Tom Hua
Date: Jan. 5, 2019

/***************************************************************************
Part I.

 - This classical recursion algorithm calls the same functon recursively.
 - Exmample, fib(8) = fib(7) + fib(6) and is expanded until reaching the base.
 like: fib(8) = (fib(6) + fib(5) + (fib(5) + fib(4))
       fib(8) = ... + ... + ... + ...
       fib(8) ... + ... + (fib(1) + fib(0))
 - That is, each successive expansion calls the same fib(n) redundantly.
 - Each call pushes the current variable on to the stack, which allocates memory 
   and needs a memory access.
 - Each stack push/pop requires the OS to manage the the process, registers, 
   state and program counter, and function parameters, etc. That's expensive.
 - Each return of call pops the previously saved variable from the stack,
   which is again a memory access to transfer the value from memory to register.
 - As the n given is larger and larger, the number of push/pop increases and 
   more memory is needed.
 - Thus the time it takes accumulates accordingly.
 - The larger the n, the stack level grows deeper, the more memory is needed,
   and the more reduncy of fuction call it takes.
 - Worse yet, if the stack level is not carefully designed, stack overflow could
   cause system crash due to illegal memory access, and out-of-memory exception.
     
 - The iteration algorithm uses the same memory all along. If the memory access
   is within the CPU cache, that's is even faster than regular access.
   (gladly, I took the kernel course that covers the TLB for memory management.)
   And no redundancy of calling the same fib(n). 
   Thus, both speed and memory usage are better. 
 - The complexity of iteration is O(n), linearly looping through by n times.
 - The complexity of recursion is about O(2^n) when the n is large by redundancy.  
 
 Part II.
 
 - To conclude this Fibonacci recursion with big number is more expensive
   than the iteration.
 - If the n is considerbly small then it's OK to use this recursion method 
   within an acceptable execution time and amount of memory requirement. 
   
 - I suggest use iteration method in embedded system, specially robotics 
   system where precess speed is very important. I avoid recursion not ony that   
   recursive functions must be designed for reentrancy, also interrupts are 
   highly in use that will drag down the performance of recursion due to context
   switching.       
 - For comparision and choices in general, we discuss an algorithm in terms of 
   space or speed optimized, in terms of complexity, and memory requirement, in 
   terms of test-driven design. Unless recursion shows that it offers lot less 
   memory foot print to save precious program memory segment. So iteration 
   method is my choice; it is easier to be debugged and maintained.
      
 - There are some cases such as Depth First Search algorithm where iteration 
   needs more line of codes than the recursion. There are cases where the problem
   is recursive in nature. Like, Fibonacci sequence and Factorial numbers that
   the final answer is rendered from the sub answers or previous answers. For 
   representing the problem's data structure and proving the algorithm works, 
   then it's OK to use recursion.
 
 - This classical Fibonacci recursion might be optimized to work faster by rid
   of repeating the Fib(n-1), Fib(n-2), or use the bottom-up approach to build
   from the previous answer by storing it at first then reuse it. Worth to 
   explore further.
    
 - tomhua_test3.c has three test cases for three different approaches. It shows 
   the approximate time of execution and number of calls to profile the speed.
 - The recursion and lookup table test case limits the n to 45 so that the test 
   does not take so long to run. (the iteration still allows n up 100.)
 - To print out correctly I use hex format for any Fibonacci number bigger than 
   the printf can process decimal format. (limits.h defines ULONG_MAX).
 - I run the test on i3 and much faster i5 CPU.
 - I use a separate test program so that the algorithm can be distributed.
 - I attach snapshots of program runs on the PC, C code set and test program.
 
 Part III.
    
 - There are few other approach such as use of lookup table if the n is small.
   THis approach costs more statically allocated memory, yet only one line of 
   code is needed to get the Fibonacci number directly and quickly without 
   calculation. The cost to understand it and maintaining is minimum.
 - Through online serch, some compiler support such as 'tail-call recursion'.
   Worth to explore further.
 
 
  
       
****************************************************************************/


