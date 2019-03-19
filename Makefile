CC = g++
CFLAGS = -g -c
STANDARD = -std=c++11


all: hw3 

hw3: hw3.o
	$(CC) -o $@ $? 

hw3.o:	hw3.cpp
	$(CC) $(STANDARD) $(CFLAGS) -o $@ hw3.cpp 

clean:
	rm *.o  hw3 

