all:
	rm -rf test_stack
	gcc -std=gnu99 -g -O0 object.h object.c stack.h stack.c  stack_test.c -o test_stack