#include <stdio.h>

int globalA = 10;	// Data Segment
int globalB;		// Data (bss) Segment

int main()
{
	int i = 10;		// Stack Segment

	char* str = "Iam a CSE student!!";	// We don't know

	printf("globalA address: %p\n", &globalA);	
	printf("globalB address: %p\n", &globalB);
	printf("i address: %p\n", &i);
	printf("str address: %p\n", &str);

	// str[0] = 'M';

	printf("str: %s\n",str);

	for(;;) ;		// Infinite Loop
	return 0;
}
