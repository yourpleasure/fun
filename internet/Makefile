CC = gcc
CFLAG = -Wall -g
LDFLAG = -Wall
objects = main.o
main : $(objects)
	$(CC) -o main $(objects) $(LDFLAG)
main.o : main.c
	$(CC) -c main.c $(CFLAG)
.PHONY : clean
clean :
	-rm main $(objects)
