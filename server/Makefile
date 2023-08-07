OBJECTS = main.o server.o
CC = gcc

all: main 

main: $(OBJECTS)
	$(CC) -o main $(OBJECTS)


main.o: main.c
	$(CC) -c main.c		

server.o: server.c
	$(CC) -c server.c

clean:
	rm main 
	rm -rf .rep/
	rm *.o

run: all
	./main
				
init: all
	./main init .
						
