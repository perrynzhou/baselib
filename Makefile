all:
	rm -rf test_*
	#gcc -std=gnu99 -g -O0 object.h object.c stack.h stack.c  stack_test.c -o test_stack
	#gcc -std=gnu99 -g -O0 object.h object.c queue.h queue.c  queue_test.c -o test_queue
	gcc -std=gnu99 -g -O0 ./inc/slice.h  ./src/slice.c  slice_test.c  -o test_slice
	#gcc -std=gnu99  -DTEST -g -O0 lru_cache.h lru_cache.c -o test_cache
	#gcc -std=gnu99  -DTEST -g -O0 list.h list.c -o test_list
	#gcc -std=gnu99  -DTEST -g -O0 slice.h slice.c -o test_slice