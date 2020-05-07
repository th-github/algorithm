/** Author: Tom Hua  
	numbers.c demonstgrate some practice on data structure 
**/

#include <stdio.h>
#include <stdlib.h>

#define min(x1, x2) x1<x2 ? x1 : x2
#define max(x1, x2) x1>x2 ? x1 : x2
#define SIZEOF_INTARR(list)    (int)(sizeof(list)/sizeof(int))

void quickSort(int arr[], int left, int right); 

// rotate (reverse) an array
void RotateA(int* array, int size);
// find the missing positive number in a range from an unsorted list
void  test_FindMissing(void);
// reverse all digits of an interger
int  RotateDigits(int n);
// find duplicated numbers in a range from an unsorted list
void test_FindRepeated(void);   // hashing
void test_FindDuplicate(void);  // linear O(N^2)
void getfreqencylist(void);
void test_findthePeak(void);

void test_cwrotate2Darr(void);  // clockwise rotate 2D array

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
       
    // this sorted list is used for searching x and returns its index 
    int SortedList[10] = {3, 10, 16, 21, 27, 32, 33, 44, 56, 57};

	printf("----- Tom Hua's data structure practice -----\n");
	////////////////////////////////////
    int n = (int)(sizeof(array)/sizeof(int));
    printf("RotateA(original array): ");
    for (i = 0; i < n; i++)
        printf("%3d ", array[i]);
    printf("\n");
    RotateA(array, 6);
    printf(" rotated array: ");
    for (i = 0; i < n; i++)
        printf("%3d ", array[i]);
    printf("\n\n");
	///////////////////////////////////

    test_FindMissing();
    printf("\n");
	///////////////////////////////////

    i = 94041;
    n = RotateDigits(i);
    printf(" RotateDigits() '%d': %d\n", i, n);
    printf("\n");
	///////////////////////////////////

    printf("FindMaxPairSum(): Find largest pair sum from an array: ");
    n = (int)(sizeof(array)/sizeof(int));
    for (i = 0; i < n; i++)
        printf("%2d ", array[i]);
    printf("\n");
    n = FindMaxPairSum(array, n);
    printf(" largest pair sum: %d\n", n);
    printf("\n");
	////////////////////////////////////

    printf("FindGreatestXOR(): Find largest XOR value of each pair from list:\n");
    n = SIZEOF_INTARR(array);
    for (i = 0; i < n; i++)
        printf("%2d ", array[i]);
    printf("\n");
	
    n = FindGreatestXOR(array, n);
    printf(" greatest pair XOR: %d (%0x)\n", n, n);
    printf("\n");
	/////////////////////////////////////

    i = -1;
    printf("GetPositionOfX(SortedList):\n");
    i = GetPositionOfX(SortedList, 1, 8, 44);
    printf(" index of 44: %d\n", i);
    i = GetPositionOfX(SortedList, 0, 9, 3);
    printf(" index of 3: %d\n", i);
    i = GetPositionOfX(SortedList, 0, 9, 57);
    printf(" index of 57: %d\n", i);
    i = GetPositionOfX(SortedList, 9, 9, 57);
    printf(" index of 57: %d\n", i);
    printf("\n");  
    ///////////////////////////////////
      
    test_FindRepeated();
    printf("\n");
	///////////////////////////////////
	getfreqencylist();
	printf("\n");
	///////////////////////////////////
	
	test_FindDuplicate();
	printf("\n");
	///////////////////////////////////
		
	test_findthePeak();
	///////////////////////////////////
	printf("\n");
	
	test_cwrotate2Darr();
	printf("\n");
	///////////////////////////////////
	
    printf("----------------------------------\n");
    return 0;
}

//////////////// helpers /////
void quickSort(int arr[], int left, int right) 
{
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];

      /* partition */
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;

        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }; 

    /* recursion */
    if (left < j)
        quickSort(arr, left, j);
    
    if (i < right)
        quickSort(arr, i, right);
}
//////////////////////////////

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
//   so the sum of [1:n] subtract the total renders the difference
void  test_FindMissing(void)  // from the array having an empty spot
{
// all values are within 1 ... 10, this list has the missing number. 
    int array[10] = {8, 3, 2, 9, 1, 7, 4, 6, 10};
    int size = 10;  
   // assume positive values in array
    int missingNum = -255;
    int i, total = 0;
    int sum = (size * (size+1)) / 2; // add all numbers in the range of size

    int n = size-1;  // one less than the size of array because one empty spot
    printf("numbers in this list:\n");
    for (i = 0; i < n; i++) 
    {
        printf(" %d,", array[i]);
        total += array[i];
    }
    missingNum = sum - total;
    printf("\n");
    printf(" FindMissing() a number in this list: %d\n", missingNum);
    //return missingNum;
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

void test_FindRepeated(void)
{   // assum value in the array is >= 1 and <= array size
    // this list contains duplicates. all values are within 1 ... 20
    int array[20]  = {7, 20, 8, 10, 12,  15, 3, 8, 12, 5,
                      5, 11, 2, 9, 6,    20, 17,13, 4, 1};
    static int dup[22];  // this list holds the duplicates found from list 'dup'
    int size = 20, i, index = 0;    
       
    printf("numbers each occurs once:\n");
    for (i = 0; i < size; i++)
    {
        // borrow a position of the array to form an non-negative index
        // The index formed starts from 0 thus subtract 1 so the first
        index = abs(array[i]) - 1;  // need to turn it into positive
        if (array[index] < 0)   // check if the content has been negated
        {
            dup[index] = abs(array[i]);   // found, save this duplicate
        }
        else {
            array[index] = -array[index];   // negate the content
            if (array[i] > -1)
                printf("[%d]:%d, ", i, array[i]);
        }
    }
    printf("\n");
    printf("duplicates:\n");
    for (i = 0; i < size; i++)
    {
        if (dup[i]>0)
            printf("%d, ", dup[i]);
    }
    printf("\n");
}

void getfreqencylist(void)
{   // assum value in the array is >= 0 and <= array size
    // this list contains duplicates. all values are within 0 ... 20
    int array[20]  = {7, 15, 12, 19, 8,  0, 19, 16, 12, 5,
                      15, 12, 0, 19, 8,  0, 20, 13, 20, 5};      
    static int dup[22];  
    int size = 20, i, index = 0;    
    int n = 0;  // track repeated element value of 0
       
    for (i = 0; i < size; i++)
    {
        if (array[i] == 0)  // handle special case
        {
            if (n++ > 0)
                dup[0] += 1; 
            continue;
        }

        // borrow a position of the array to form an non-negative index
        // The index formed starts from 0 thus subtract 1 so the first
        index = abs(array[i])-1;  // need to turn it into positive
        // dup[0] is for duplicated 0
        if (array[index] < 0)   // check if the content has been negated
            dup[index+1] += 1;    // found, save this duplicate
        else 
            array[index] = -array[index];   // negate the content        
    }

    size++; // covers up to highest element value == size of list
    printf("list of [element]: frequency\n");
    for (i = 0; i < size; i++)
    {   // dup[] was initialized 0, so add one to it, dupliates means 2 or more
        if (dup[i]>0) 
            printf("[%d]: %d, ", i, dup[i]+1);
    }
    printf("\n");
}

void test_FindDuplicate(void)
{
    int arr[20]  = {7, 20, 8, 10, 12,  15, 3, 8, 12, 5,
                      5, 11, 2, 9, 6,    20, 17,13, 4, 1};   
    
    int c, n;   // current and next
    int size = SIZEOF_INTARR(arr);
    printf("duplicates:\n");
    for (c = 0; c < size; c++) {         
        for (n = 0; n < size; n++) {
            if (c == n) continue; // skip self compare
            if (arr[c] == arr[n])
            printf("dup:%d @%d, ", arr[c], c);
        }
    } 
    printf("\n");                                  
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

void test_findthePeak(void)
{   // assume only one peak, no up and down in between
    unsigned int a[15] = {4,10,12,13,15, 22,21,20,18,9, 7,6,4,3,2};
    int index = -1, p = -1;
    int start = 0, end = 14, m;

    for (int i = 0; i <= end; i++)
        printf("%d, ", a[i]);
    printf("\n");   
    
    while(start <= end) {
        m = (end + start)/2;  // the sliding range gets smaller each loop
        printf("start:%d, m:%d, end:%d\n", start, m, end);
        
        if (m < 1) {     // must be the first one
            index = 0;
            p = a[0];
            break;
        }
        if (m == end) {
            index = end; // must be the last one
            p = a[end];
            break;
        }
        
        if (a[m-1] < a[m])  // m is in ascending slope
        {                   
            start = m+1;    // so start from higher point 
            if (a[m] > a[m+1]) {
                index = m;
                p = a[m];
                break;
            }
            printf("start:%d\n", start);
        }
        else
        if (a[m] > a[m+1])  // m is in descending slope
        {
            end = m-1;      // so search backward away from the end 
            if (a[m-1] < a[m]) {
                 index = m;
                 p = a[m];
                 break;  
            }  
            printf("end:%d\n", end);        
        }
        else    // there is dip in between
        {   // walk either way, toward to back, or           
            end--;  // say toward the front  
        }
    }
    printf(" peak:%d at %d\n", p, index);
}

void test_cwrotate2Darr(void)  // clockwise rotate 2D array
{
    int arr[4][4] = {11,12,13,14, 21,22,23,24, 31,32,33,34, 41,42,43,44};
    int r, c, n = 4, temp;
    
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++)
            printf("%d ", arr[r][c]);
        printf("\n");
    }
    printf("cw op:\n");
    for (r = 0; r < n; r++) {
        for (c = r; c < (n-1-r); c++) {
            temp = arr[r][c];   // save the first one @[r][c]
            arr[r][c] = arr[n-1-c][r];      // cw move #1, to upper left 
            arr[n-1-c][r] = arr[n-1-r][n-1-c];  // cw move #2, to lower left
            arr[n-1-r][n-1-c] = arr[c][n-1-r];  // cw move #3, to lower right
            arr[c][n-1-r] = temp;   // cw move #4, to upper right
        }
    }
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++)
            printf("%d ", arr[r][c]);
        printf("\n");
    }
    printf("\n");
}


