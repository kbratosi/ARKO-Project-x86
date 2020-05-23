CC = g++
CFLAGS = -Wall -m64

all: main.o f_c.o
	$(CC) $(CFLAGS) -o fun main.o f_c.o -lglut -lGLU -lGL

asm: main.o f_a.o
	$(CC) $(CFLAGS) -o fun main.o f_a.o -lglut -lGLU -lGL

f_a.o: f.s
	nasm -f elf64 -o f_a.o f.s

f_c.o: f.cpp
	$(CC) $(CFLAGS) -c -o f_c.o f.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c -o main.o main.cpp

clean:
	rm -f *.o
