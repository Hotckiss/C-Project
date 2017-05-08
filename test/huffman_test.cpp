#include "huffman_test.h"

int huffman_test::total_num;
int huffman_test::failed_num;

void huffman_test::check(bool expr, const char *func, const char *filename, size_t line_num) {
	if (!expr) {
		std::cerr << "test failed: " << func << " in " << filename << ":" << line_num << "\n";
		failed_num++;
	}
	total_num++;
}

void huffman_test::show_final_result() {
	if (!failed_num) 
		std::cerr << "All " << total_num << " test passed.\n";
	else 
		std::cerr << "Failed " << failed_num << " of " << total_num << " tests.\n";
}
