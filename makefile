CC= g++
CFLAGS= -c -Wall

all: ksim

ksim: main.cpp processes.cpp processes.h commands.cpp
	$(CC) -o ksim main.cpp processes.cpp commands.cpp

clean:
	rm -rf *o ksim