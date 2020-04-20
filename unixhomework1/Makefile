clock : main.o localtime.o localdate.o elapsedtime.o
	gcc -g -Wall -o clock main.o localtime.o localdate.o elapsedtime.o -lncurses -lpthread

main.o : main.c
	gcc -c -o main.o main.c

localtime.o : localtime.c
	gcc -c -o localtime.o localtime.c

localdate.o : localdate.c
	gcc -c -o localdate.o localdate.c

elapsedtime.o : elapsedtime.c
	gcc -c -o elapsedtime.o elapsedtime.c

clean :
	rm *.o clock
