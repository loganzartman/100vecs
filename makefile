.SUFFIXES:

CC = gcc --std=c11

.PHONY: all
all: lilvecs_test function_list.md

.PHONY: clean
clean: 
	rm -f *.o lilvecs_test lilvecs_test.exe function_list.md

.PHONY: test
test: lilvecs_test
	./lilvecs_test

lilvecs_test: lilvecs_test.c lilvecs.h
	$(CC) lilvecs_test.c -o lilvecs_test

function_list.md: lilvecs.h
	printf '#include "lilvecs.h"\nVEC_DECL(T)' > function_list.c
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
