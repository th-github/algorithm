
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

unsigned int reversedigits(unsigned int v);
unsigned int findlostnum(unsigned int *);
int findrepeatednum(int [], int, unsigned int*);
void getfreqencylist(int list[], int size, int* freq);
 
// toggle all middle bits 
int main(void)
{
	unsigned int i = 1234; 
	unsigned int ret = 0;
    static int freq[16];
    
	cout << "-----------------" << endl;
	cout << "reversedigits(unsigned int v): " << i << endl;
	ret = reversedigits(i);
	cout << i << " -> " << ret << endl;

	unsigned int list[15] = {6,15,4, 7,12,2, 9,11,8, 14,13,1, 5,4};
	cout << "sum of 1...15 = " << findlostnum(list) << endl;

	int dup[15] = {6,15,4, 15,12,6, 9,11,15, 3,13,6, 15,4,14};
	// element:frequency -- 6:3, 15:4, 4:2 
	cout << "------------" << endl;

	cout << "findrepeatednum(dup, 15)" << endl;
	i = findrepeatednum(dup, 15, &ret);
	cout << "duplicated number: " << i << " @ " << ret << endl;
    cout << "------------" << endl;
    
    getfreqencylist(dup, 15, freq);
    
	cout << "------------" << endl << endl;
	return 0;
}


unsigned int reversedigits(unsigned int v)
{
	unsigned remainder = 0, temp = 0;
	while (v > 0) {
		remainder = v % 10;	// retrieve the least digit
		temp = (temp * 10) + remainder;	// increase the sume by 10
		v /= 10;	// get the next least digit
	}
	return temp;	
}

unsigned int findlostnum(unsigned int list[])
{
	unsigned int i, n = 0, m = 0;
	
	n = (unsigned int)(15*(15+1)/2);

	for (i = 0; i < 14; i++)
		m += list[i];

	m = n - m;
	return m;

}


int findrepeatednum(int list[], int size, unsigned int* position)
{
	int dup = 0;
	int i, index = 0;

	for (i = 0; i < size; i++)
	{
		index = abs(list[i]) - 1; // use the element value as an index
		if (list[index] < 0) {
			dup = abs(list[i]);	  // duplicate, because it was negated before
			*position = i;
			break;
		}
		else
			list[index] = -(list[index]);
	}
	return dup;

}

void getfreqencylist(int list[], int size, int* freq)
{   // assume the freq[] was initialized zero, and has size+1
	int i, index = 0;

	for (i = 0; i < size; i++)
	{
		index = abs(list[i])-1; // use the element value as an index
		if (list[index] < 0) {  // it duplicated because it was negated before
			freq[index+1] += 1;	// save the count at the index of the element
		}
		else    // it's a value not found before
			list[index] = -(list[index]);   // so negate it. 
	}
	size++; // this includes the biggest element value == size 
	cout << "list of element frequency:" << endl;
	for (i = 0; i < size; i++) {
	    if (freq[i] > 0)
	        cout << i << " count: " << freq[i] << ", ";
	}
	cout << endl;    
}
