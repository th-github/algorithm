
#include <stdio.h>
#include <string.h>

void reversestr(char *);
int  strcompare(char*, char*);
void test_swaptwonumbers(void);

void main(void)
{
	char str1[20] = "TomJerrie";
	char str2[20] = "eirreJmoT";

	reversestr(str1);
	printf("%s\n", str1);

	printf("compare str: %d\n", strcompare(str1, str2));

    test_swaptwonumbers();
}

void reversestr(char* str)
{
	char c;
	int len = strlen(str) - 1;
	int m = strlen(str) / 2;
	int n = 0;

	for( n = 0; n < m; len--, n++) {
		c = *(str + len);
			*(str + len) = *(str + n);
		*(str + n) = c;
	}
}

int  strcompare(char* s1, char* s2)
{
	while (*(s1++) == *(s2++)) {
		if (*s1 == '\0' && *s2 == '\0')
			return 0;
	}
		 
	return -1;
}

void test_swaptwonumbers(void)
{
    int x = 10, y = 600;
    printf("x:%d, y=%d\n", x, y);
    x ^= y;
    y ^= x;
    x ^= y;
    printf("swapped x:%d, y=%d\n", x, y);
}


