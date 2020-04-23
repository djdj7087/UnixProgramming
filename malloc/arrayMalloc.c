#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	int 	studentID;
	char 	name[10];
};

int main()
{
	int *studentID;

	// We need type cast
	studentID = (int *)malloc(sizeof(int) * 3);

	if(studentID == NULL) {
		fprintf(stderr, "Out of memory!!\n");
		exit(-1);
	}

	studentID[0] = 12345;
	studentID[1] = 23456;
	studentID[2] = 34567;

	printf("%d\n", studentID[0]);
	printf("%d\n", studentID[1]);
	printf("%d\n", studentID[2]);
	
	free(studentID);
	
	struct student *pStudent;

	// We need type cast
	pStudent = (struct student *)malloc(sizeof(struct student));

	if(pStudent == NULL) {
		fprintf(stderr, "Out of memory!!\n");
		exit(-1);
	}

	pStudent->studentID = 98765; // same as (*pStudent).studentID = 98765;
	printf("%d\n", pStudent->studentID);
	// printf("%d\n", (*pStudent).studentID);
	
	strcpy(pStudent->name, "PNU CSE");
	printf("%s\n", pStudent->name);

	free(pStudent);

	struct student *studentList;
	
	// looks like struct student studentList[10]; on the stack
	// 											  fixed size

	// Array on the heap: Dynamic size: Why? You can increase / decrease
	// additional array size using realloc()
	studentList = (struct student *)malloc(sizeof(struct student) * 10);
	
	if(studentList == NULL) {
		fprintf(stderr, "Out of memory!!\n");
		exit(-1);
	}

	studentList[0].studentID = 2020345;
	strcpy(studentList[0].name, "Jico");
	
	printf("%d\n", studentList[0].studentID);
	printf("%s\n", studentList[0].name);

	free(studentList);

	return 0;
}
