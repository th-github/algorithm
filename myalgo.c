#include <stdio.h>
#include <stdlib.h>

#define min(x1, x2) x1<x2 ? x1 : x2
#define max(x1, x2) x1>x2 ? x1 : x2
#define SIZE(list)    (int)(sizeof(list)/sizeof(int))

// rotate (reverse) an array
void RotateA(int* array, int size);
// find the missing positive number in a range from an unsorted list
int  FindMissing(int* array, int size);
// reverse all digits of an interger
int  RotateDigits(int n);
// find duplicated numbers in a range from an unsorted list
int  FindRepeated(int* array, int size, int* dup);
// find the greates sum of a pair of element from a list
int  FindMaxPairSum(int* array, int size);
// find the largest value of XOR a pair element from a list
int  FindGreatestXOR(int* list, int size);
// find the position of a number from a sorted list
int  GetPositionOfX(int* array, int low, int high, int x);

int main(void)
{
    int i = 0;
    int array[6] = {12, 8, 28, 9, 25, 0};
    
    // this list has the missing 5. all values are within 1 ... 10 
    int list[10] = {8, 3, 2, 9, 1, 7, 4, 6, 10};
    
    // this list contains duplicates. all values are within 0 ... 19
    int dup[20]  = {7, 19, 8, 0, 12, 15, 3, 8, 12, 5,
                    5, 11, 2, 9, 6,  9, 17,13, 4, 0};
    int ret[20];  // this list holds the duplicates found from list 'dup'
    
    // this sorted list is used for searching x and returns its index 
    int SortedList[10] = {3, 10, 16, 21, 27, 32, 33, 44, 56, 57};

    int n = (int)(sizeof(array)/sizeof(int));
    printf("original array: ");
    for (i = 0; i < n; i++)
        printf("%3d ", array[i]);
    printf("\n");
    RotateA(array, 6);
    printf(" rotated array: ");
    for (i = 0; i < n; i++)
        printf("%3d ", array[i]);
    printf("\n\n");

    n = (int)(sizeof(list)/sizeof(int)) - 1;
    printf("original list:\n");
    for (i = 0; i < n; i++)
        printf("%3d ", list[i]);
    printf("\n");

    // find the missing 5
    n = FindMissing(list, 10);
    printf(" missing number in this list: %d\n", n);
    printf("\n");

    i = 94041;
    n = RotateDigits(i);
    printf(" Rotated '%d': %d\n", i, n);
    printf("\n");

    printf(" An array having one or more duplicated number\n");
    n = (int)(sizeof(dup)/sizeof(int)) - 1;
    for (i = 0; i < n; i++)
        printf("%2d ", dup[i]);
    printf("\n");

    n = 0;  // n holds number of duplcates found
    n = FindRepeated(dup, 20, ret);  // ret holds the duplicates
    printf(" count of duplicates found: %d\n", n);
    printf(" duplicates: ");
    for (i = 0; i < n; i++)
        printf("%2d ", ret[i]);
    printf("\n\n");

    printf("Find largest pair sum from an array: ");
    n = (int)(sizeof(array)/sizeof(int));
    for (i = 0; i < n; i++)
        printf("%2d ", array[i]);
    printf("\n");
    n = FindMaxPairSum(array, n);
    printf(" largest pair sum: %d\n", n);
    printf("\n");

    printf("Find greatest XOR value of each pair from a list:\n");
    n = SIZE(array);
    for (i = 0; i < n; i++)
        printf("%2d ", array[i]);
    printf("\n");

    n = FindGreatestXOR(array, n);
    printf(" greatest pair XOR: %d (%0x)\n", n, n);
    printf("\n");

    i = -1;
    printf(" find index of x:\n");
    i = GetPositionOfX(SortedList, 1, 8, 44);
    printf(" index of 44: %d\n", i);
    i = GetPositionOfX(SortedList, 0, 9, 3);
    printf(" index of 3: %d\n", i);
    i = GetPositionOfX(SortedList, 0, 9, 57);
    printf(" index of 57: %d\n", i);
    i = GetPositionOfX(SortedList, 9, 9, 57);
    printf(" index of 57: %d\n", i);
        

    printf("\n\n");
    return 0;
}

void RotateA(int* array, int size)
{
    int i;
    int temp;
    int length = size - 1;
    int m = (int)(size / 2);

    for (i = 0; i < m; i++, length--)
    {
        temp = array[length];
        array[length] = array[i];
        array[i] = temp;
    }
}

// algo: the total of all elements must be the exact difference than the sum of 1 ... n
//     so the sum of [1:n] subtract the total renders the difference
int  FindMissing(int* array, int size)  // from the array having an empty spot
{   // assume positive values in array
    int missingNum = -255;
    int i, total = 0;

    int sum = (size * (size+1)) / 2;    // add all numbers in the range of size

    for (i = 0; i < size-1; i++) // one less than the size of array because one empty spot
    {
        total += array[i];
    }
    missingNum = sum - total;

    return missingNum;
}

int  RotateDigits(int n)
{
    int digits = 0, remainder = 0;
    while (n > 0)
    {
        remainder = n % 10;
        digits = digits* 10 + remainder;
        n /= 10;
    }
    return digits;
}

//
int  FindRepeated(int* array, int size, int* dup)
{   // assum value in the array is >= 1 and <= array size
    int i, count = 0, index = -1;
    int n = 0;  // count for element value == 0
    for (i = 0; i < size; i++)
    {
        if (array[i] == 0)  // handle special case
        {
            n++;
            continue;
        }
        // borrow a position of the array. the element value is assumed >= 1
        // The position constructed would be 0 minimum thus subtract 1 so the first
        // index would be 0
        index = abs(array[i]) - 1;  // need to turn it into positive, see negation below
        if (array[index] < 0)   // check if the content has been negated
        {
            *dup = abs(array[i]);    // yes, it is, save this duplicate to the output array
            dup++;                   // move to next position
            count++;
        }
        else
            array[index] = -array[index];   // negate the content
    }
    if (n > 1) {
        *dup = 0;   // the pointer was incremeneted when duplicates were found
        count++;
    }
    return count;
}

int  FindMaxPairSum(int* array, int size)
{
    int i, first = 0, second = 0;

    first  = max(array[0], array[1]);
    second = min(array[0], array[1]);

    for (i = 2; i < size; i++)
    {
        if (array[i] > first)
        {
            second = first;
            first = array[i];
        }
        else
        {
            second = array[i];
        }
    }
    printf(" x1: %d x2: %d\n", first, second);
    return ( first + second );
}

int  FindGreatestXOR(int* list, int size)
{
    int i, j, temp, ret = 0;

    for (i = 0; i < size; i++)
    {
        temp = 0;
        for(j = i; j < size; j++)
        {   // xor one (start from the first) with the rest
            printf(" r:%0x ^ a:%0x, ",
                temp, list[j]);
            temp = (temp ^ list[j]);
            ret = max(temp, ret);   // save the max
            // repeat for the next (from the 2nd ...) with the rest
        }
        printf(" \n[%d] : %0x\n", i, ret);
    }
    return ret;
}

// find the position of a number from a sorted list
// algo: recursive binary search. decrement the high position
// until it reaches the low position or stop as soon a match is found
int  GetPositionOfX(int* list, int low, int high, int x)
{
    if (high >= low)
    {
        // the low can be higher than 0, so the range starts from the low
        int mid = low + (int)((high-low)/2);
        
        // is the one at the mid position equal to x?
        if (list[mid] == x)
            return mid; // yes

        // otherwise, find in the righ side having higher value
        if (list[mid] < x) {
            printf(" search [%d,%d]\n", low, high);
            return GetPositionOfX(list, (mid+1), high, x);
        }

        printf(" search [%d,%d]\n", low, high);
        // otherwise, find in the left side having lower value
        return GetPositionOfX(list, low, (mid-1), x);
    }

    return -1;  // could not find x.
}



