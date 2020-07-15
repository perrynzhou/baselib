all:
	rm -rf test_stack
	gcc -std=gnu99 -g -O0 object.h object.c stack.h stack.c  stack_test.c -o test_stack
	gcc -std=gnu99 -g -O0 object.h object.c queue.h queue.c  queue_test.c -o test_queue
	gcc -std=gnu99 -g -O0 slice.h  slice.c  slice_test.c  -o test_slice