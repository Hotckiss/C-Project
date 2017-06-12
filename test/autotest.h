#include "huffman_test.h"
#include "../src/huffman.h"

class double_queue_test : public huffman_test {
public:
	void test_size();
	void test_push_pop();
	void test_sort_queue();
	void run_all_tests();
};

class bit_accum_test : public huffman_test {
public:
	void test_push_pop_bit();
	void test_get_bit();
	void test_write();
	void test_read();
	void test_is_full();
	void test_empty();
	void test_clear();
	void run_all_tests();
};

class huffman_archiever_test : public huffman_test {
public:
	void test_compress_decompress();
	void test_compress_decompress2();
	void test_count_frequency();
	void test_make_queue_build_free_tree();
	void test_create_codes();
	void test_encode_decode_table();
	void test_get_log();
	void test_clear();
	void run_all_tests();
};
