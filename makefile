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

100vecs_test: 100vecs.h 100vecs_test.c 100vecs_test_nesting.c
	$(CC) 100vecs_test.c 100vecs_test_nesting.c -o 100vecs_test

function_list.md: 100vecs.h
	printf '#include "100vecs.h"\nVEC_DECL(T)' > function_list.c
	echo "\`\`\`C" > function_list.md
	$(CC) -E function_list.c -o- \
		| grep -w Vec_T \
		| sed 's/; */;\n/g' \
		| /usr/bin/sort -k3 \
		| sed 's/extern //g' \
		| awk 'NF' \
		>> function_list.md
	echo "\`\`\`" >> function_list.md
	rm function_list.c
