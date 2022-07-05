# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g -pthread
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o Graph.o Node.o
		$(CC) $(CFLAGS) -o main main.o Graph.o Node.o

# The main.o target can be written more simply

main.o: main.cpp Graph.h Node.h
		$(CC) $(CFLAGS) -c main.cpp

Node.o:  Node.cpp Node.h
		$(CC) $(CFLAGS) -c Node.cpp

Graph.o: Graph.cpp Graph.h  Node.h
		$(CC) $(CFLAGS) -c Graph.cpp
clean:
		$(RM) main main.o Graph.o Node.o