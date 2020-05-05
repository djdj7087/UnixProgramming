#include <stdio.h>
#include <pthread.h>

void *print_message_function(void *ptr);

int main()
{
	pthread_t 	thread1, thread2;
	int 		thread1Return;
	int 		thread2Return;

	char		*message1 = "Thread1 is running!!";
	char		*message2 = "Thread2 is running!!";
	
	// create threads that will execute function
	thread1Return = pthread_create(&thread1, NULL, print_message_function,
										(void*) message1);
	return 0;
}

// Function used for threads
void *print_message_function(void *ptr)
{
	char *message;

	message = (char *)ptr;
	printf("%s\n", message);
}
