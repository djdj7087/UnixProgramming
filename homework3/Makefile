chat : chat.o chatremove.o cleanup.o drawbox.o
	gcc -g -Wall -o chat chat.o chatremove.o cleanup.o drawbox.o -lncurses -lpthread

chat.o : chat.c
	gcc -c -Wall -o chat.o chat.c

chatremove.o : chatremove.c
	gcc -c -Wall -o chatremove.o chatremove.c

cleanup.o : cleanup.c
	gcc -c -Wall -o cleanup.o cleanup.c

drawbox.o : drawbox.c
	gcc -c -Wall -o drawbox.o drawbox.c

clean :
	rm *.o chat
