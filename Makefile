all: main 

main: $(wildcard *.c)
	gcc $(wildcard *.c) -o main 

clean:
	rm main 
	rm -rf .rep/
run: all
	./main
init: all
	./main init .
	
