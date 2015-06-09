SOBJS = mint.o smain.o server.o
COBJS = mint.o cmain.o client.o

ALL : server.exe client.exe

server.exe : $(SOBJS) 
	gcc -Wall -o server.exe $(SOBJS)

client.exe : $(COBJS) 
	gcc -Wall -o client.exe $(COBJS)

mint.o : code/mint.c code/mint.h
	gcc -Wall -c code/mint.c

smain.o : code/server/smain.c
	gcc -Wall -c code/server/smain.c

cmain.o : code/client/cmain.c
	gcc -Wall -c code/client/cmain.c

server.o : code/server/server.c code/server/server.h
	gcc -Wall -c code/server/server.c

client.o : code/client/client.c code/client/client.h
	gcc -Wall -c code/client/client.c 

clean :
	rm *.o
