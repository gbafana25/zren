OBJECTS = main.o ignore.o log.o menu.o storage.o track.o client.o
CC = gcc

all: zren

zren: $(OBJECTS)
	$(CC) -o zren $(OBJECTS)


main.o: main.c
	$(CC) -c main.c	

ignore.o: ignore.c ignore.h
	$(CC) -c ignore.c	

log.o: log.c log.h
	$(CC) -c log.c	

menu.o: menu.c menu.h
	$(CC) -c menu.c	

client.o: client.c client.h
	$(CC) -c client.c	


storage.o: storage.c storage.h
	$(CC) -c storage.c	

track.o: track.c track.h
	$(CC) -c track.c	

clean:
	rm zren
	rm -rf .rep/
	rm *.o

run: all
	./zren
				
init: all
	./zren init .
						
