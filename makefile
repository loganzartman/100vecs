.SUFFIXES:

.PHONY: all
all: lilvecs_test

.PHONY: clean
clean: 
	rm -f *.o lilvecs_test

.PHONY: test
test: lilvecs_test
	./lilvecs_test

lilvecs_test: lilvecs_test.c lilvecs.h
	gcc lilvecs_test.c -o lilvecs_test
