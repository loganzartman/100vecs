.SUFFIXES:

CC = gcc --std=c11

.PHONY: all
all: 100vecs_test function_list.md

.PHONY: clean
clean: 
	rm -f *.o 100vecs_test 100vecs_test.exe function_list.md

.PHONY: test
test: 100vecs_test
	./100vecs_test

100vecs_test: 100vecs.h 100vecs_test.c 100vecs_test_nesting.c 100vecs_test_map.c
	$(CC) 100vecs_test.c 100vecs_test_nesting.c 100vecs_test_map.c -o 100vecs_test

function_list.md: 100vecs.h make_function_list.sh
	printf '#include "100vecs.h"\nVEC_DECL(T)\nMAP_DECL(K,V)' > function_list.c
	$(CC) -E function_list.c -o function_list.txt
	./make_function_list.sh
	rm function_list.c function_list.txt
