chat : chat.o chatremove.o
	gcc -g -Wall -o chat chat.o chatremove.o

chat.o : chat.c
	gcc -c -Wall -o chat.o chat.c -lncurses -lpthread

chatremove.o : chatremove.c
	gcc -c -Wall -o chatremove.o chatremove.c

clean :
	rm *.o chat
