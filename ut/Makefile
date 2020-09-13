all:
	rm -rf test_*
	gcc -g -O0 ../slice.h ../slice.c slice_ut.c -o test_slice
	gcc -g -O0 -std=gnu99 ../list.h ../list.c list_ut.c -o test_list
	gcc -g -O0 -std=gnu99 ../dict.h ../dict.c dict_ut.c -o test_dict -luuid