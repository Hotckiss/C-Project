#include "autotest.h"
int main() {
	double_queue_test dqt;
	dqt.run_all_tests();
	bit_accum_test bat;
	bat.run_all_tests();
	huffman_archiever_test hat;
	hat.run_all_tests();
	huffman_test::show_final_result();
	return 0;
}
