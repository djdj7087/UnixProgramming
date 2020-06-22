#include <stdio.h>

int main()
{
	int x = 4;
	void *q = &x;
	int *p = q;

	int i = *p;	// dereference is O.K. because type match (int* = int*)
	/* dereference is failed because type mismatch (int* != void*) */
	// int j = *q;

	// So We need a type cast
	// First, void * -> (int *)
	// (int *) q
	// Second do dereference using *(...)
	// (*(int *) q)

	int j = *((int *)q); 

	return 0;
}
