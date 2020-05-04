#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// by Tom Hua
// given an array having a asending sorted order, no duplicates numbers.
// find a pair of these numbers matching the sum given x value.
// the x is equal or less than to the biggest numbers in the array 
 
int main(int argc, char* argv[])
{
    int s = 0;
    int arr[] = {4,4,5,7,11,12};
    char number[5];
    if (argc == 2) {
        strcpy(number, argv[1]);
        s = atoi(number);
        if (s > arr[5])
            s = 9;
    }    
    else
        s = 9;  
          
    int n1 = 0, n2 = 0, u = 6, l = 1;
    
    for (int i = 0; i < 6; i++) {
        if (arr[i] > s) {
            u = i-1;    // position the upper bound based on s
            break;
        }        
    }
    n1 = arr[0];    // assuming the first candiate
    for(l=1; l<u; l++) {
        for (int i = l; i < u; i++) { 
            if (arr[i] == s-n1) 
            {   // this candidate is the dirrence from the sum
                n2 = arr[i];
                break;
            }             
        }
        if (n1+n2 == s) // found in the first round of scanning
            break;
        n1 = arr[l];    // else the next number as the first candidate
    }
    if (n1+n2 == s) 
        printf("sum = n1:%d + n2:%d\n", n1, n2);
    else
        printf("no number pair found.\n");  

    return 0;
}


