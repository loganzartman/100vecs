.SUFFIXES:

CC = gcc

.PHONY: all
all: lilvecs_test

.PHONY: clean
clean: 
	rm -f *.o lilvecs_test lilvecs_test.exe

.PHONY: test
test: lilvecs_test
	./lilvecs_test

lilvecs_test: lilvecs_test.c lilvecs.h
	$(CC) --std=c11 lilvecs_test.c -o lilvecs_test
