#include "autotest.h"
#include <cstdlib>
#include <iostream>

void double_queue_test::test_push_pop() {
	double_queue q;
	huffman_node* huffman_nodes[4];
	for (int i = 0; i < 4; ++i)
		huffman_nodes[i] = new huffman_node(NULL, NULL, i, 10 * i);
	for (int i = 0; i < 2; ++i) {
		q.push(huffman_nodes[2 * i], FIRST);
		q.push(huffman_nodes[2 * i + 1], SECOND);
	}
	DO_CHECK(huffman_nodes[0]->ch_freq == q.extract_min()->ch_freq);
	DO_CHECK(huffman_nodes[1]->ch_freq == q.extract_min()->ch_freq);
	DO_CHECK(huffman_nodes[2]->ch_freq == q.extract_min()->ch_freq);
	DO_CHECK(huffman_nodes[3]->ch_freq == q.extract_min()->ch_freq);
	for (int i = 0; i < 4; ++i)
		delete huffman_nodes[i];
}

void double_queue_test::test_sort_queue() {
	double_queue q;
	huffman_node* huffman_nodes[100];
	for (int i = 0; i < 100; ++i)
		huffman_nodes[i] = new huffman_node(NULL, NULL, rand() % 255, rand());
	for (int i = 0; i < 50; ++i) {
		q.push(huffman_nodes[2 * i], FIRST);
		q.push(huffman_nodes[2 * i + 1], SECOND);
	}
	q.sort_queue();
	huffman_node *prev = q.extract_min(), *cur;
	for (int i = 0; i < 99; ++i) {
		cur = q.extract_min();
		DO_CHECK(cur->ch_freq >= prev->ch_freq);
		prev = cur;
	}
	for (int i = 0; i < 100; ++i)
		delete huffman_nodes[i];
}

void double_queue_test::run_all_tests() {
	test_size();
	test_push_pop();
	test_sort_queue();
}

void huffman_archiever_test::test_get_log() {
	huffman_archiever ha;
	DO_CHECK(ha.get_log(1) == 0);
	DO_CHECK(ha.get_log(2) == 1);
	DO_CHECK(ha.get_log(4) == 2);
	DO_CHECK(ha.get_log(8) == 3);
	DO_CHECK(ha.get_log(16) == 4);
	DO_CHECK(ha.get_log(7) == 3);
	DO_CHECK(ha.get_log(9) == 4);
	DO_CHECK(ha.get_log(15) == 4);
}

void huffman_archiever_test::test_count_frequency() {
	huffman_archiever ha;
	ha.count_frequency("test_file_in.txt");
	DO_CHECK(ha.freq['z'] == 7);
}

void huffman_archiever_test::test_clear() {
	huffman_archiever ha;
	ha.input_size = 1;
	ha.compressed_size = 1;
	ha.additional_size = 1;
	ha.clear();
	DO_CHECK(ha.input_size == 0);
	DO_CHECK(ha.compressed_size == 0);
	DO_CHECK(ha.additional_size == 0);
}

void huffman_archiever_test::test_make_queue_build_free_tree() {
	huffman_archiever ha;
	ha.count_frequency("test_file_in.txt");
	ha.make_queue();
	huffman_node* root = ha.build_tree();
	DO_CHECK(root->right->ch == 'a');
	ha.free_tree(root);
}

void huffman_archiever_test::test_create_codes() {
	huffman_archiever ha;
	ha.count_frequency("test_file_in.txt");
	ha.make_queue();
	huffman_node* root = ha.build_tree();
	ha.create_codes(root, 0, "");
	DO_CHECK(ha.codes['a'] == "1");
	DO_CHECK(ha.codes['z'] == "01");
	ha.free_tree(root);
}

void huffman_archiever_test::test_encode_decode_table() {
	huffman_archiever ha;
	ha.count_frequency("test_file_in.txt");
	long ans[ascii_size];
	for (int i = 0; i < ascii_size; ++i)
		ans[i] = ha.freq[i];
	std::ofstream ofs("test_file_out.txt", std::ofstream::binary);
	ha.encode_table(ofs);
	ofs.close();
	std::ifstream ifs("test_file_out.txt", std::ifstream::binary);
	ha.decode_table(ifs);
	for (int i = 0; i < ascii_size; ++i) {
		DO_CHECK(ans[i] == ha.freq[i]);
	}
}

void huffman_archiever_test::test_compress_decompress() {
	huffman_archiever ha;
	ha.compress("test_file_in.txt", "test_file_out.txt");
	ha.decompress("test_file_out.txt", "test_answer_encode.txt");
	std::ifstream ifs1("test_file_in.txt", std::ifstream::binary);
	std::ifstream ifs2("test_answer_encode.txt", std::ifstream::binary);
	char buf1[1024], buf2[1024];
	do {
		ifs1.read(buf1, sizeof(buf1));
		ifs2.read(buf2, sizeof(buf2));
		for (int i = 0; i < std::min(ifs1.gcount(), ifs2.gcount()); ++i)
			DO_CHECK(buf1[i] == buf2[i]);
	} while (ifs1.gcount() > 0 && ifs2.gcount() > 0);
}

void huffman_archiever_test::test_compress_decompress2() {
	huffman_archiever ha;
	ha.compress("ts.pdf", "test_file_out.txt");
	ha.decompress("test_file_out.txt", "tso.pdf");
	std::ifstream ifs1("ts.pdf", std::ifstream::binary);
	std::ifstream ifs2("tso.pdf", std::ifstream::binary);
	char buf1[1024], buf2[1024];
	do {
		ifs1.read(buf1, sizeof(buf1));
		ifs2.read(buf2, sizeof(buf2));
		for (int i = 0; i < std::min(ifs1.gcount(), ifs2.gcount()); ++i)
			DO_CHECK(buf1[i] == buf2[i]);
	} while (ifs1.gcount() > 0 && ifs2.gcount() > 0);
}

void huffman_archiever_test::run_all_tests() {
	test_count_frequency();
	test_make_queue_build_free_tree();
	test_create_codes();
	test_encode_decode_table();
	test_get_log();
	test_clear();
	test_compress_decompress();
	test_compress_decompress2();
}

void bit_accum_test::test_push_pop_bit() {
	bit_accum ba;
	ba.push_bit(1);
	DO_CHECK(ba.empty() == false);
}

void bit_accum_test::test_is_full() {
	bit_accum ba;
	for (int i = 0; i < 7; ++i)
		ba.push_bit(1);
	DO_CHECK(ba.is_full() == false);
	ba.push_bit(1);
	DO_CHECK(ba.is_full() == true);

}

void bit_accum_test::test_empty() {
	bit_accum ba;
	DO_CHECK(ba.empty() == true);
	ba.push_bit(1);
	DO_CHECK(ba.empty() == false);
}

void bit_accum_test::test_clear() {
	bit_accum ba;
	for (int i = 0; i < 8; ++i)
		ba.push_bit(1);
	ba.clear();
	DO_CHECK(ba.empty() == true);
}

void bit_accum_test::test_get_bit() {
	bit_accum ba;
	std::ifstream ifs("test_file_in.txt", std::ifstream::binary);
	ba.read(ifs);
	DO_CHECK(ba.get_bit(ifs) == 0);
	DO_CHECK(ba.get_bit(ifs) == 1);
	DO_CHECK(ba.get_bit(ifs) == 1);
	DO_CHECK(ba.get_bit(ifs) == 0);
}

void bit_accum_test::test_write() {
	bit_accum ba;
	std::ifstream ifs("test_file_in.txt", std::ifstream::binary);
	ba.read(ifs);
	std::ofstream ofs("test_file_out.txt", std::ofstream::binary);
	ba.write(ofs);
	ifs.close();
	ifs.open("test_file_out.txt", std::ifstream::binary);
	ba.read(ifs);
	DO_CHECK(ba.get_bit(ifs) == 0);
}

void bit_accum_test::test_read() {
	bit_accum ba;
	std::ifstream ifs("test_file_in.txt", std::ifstream::binary);
	ba.read(ifs);
	DO_CHECK(ba.get_bit(ifs) == 0);
}

void bit_accum_test::run_all_tests() {
	test_push_pop_bit();
	test_get_bit();
	test_write();
	test_read();
	test_is_full();
	test_empty();
	test_clear();
}

void double_queue_test::test_size() {
	double_queue q;
	huffman_node* huffman_nodes[10];
	for (int i = 0; i < 5; ++i) {
		q.push(huffman_nodes[2 * i], FIRST);
		q.push(huffman_nodes[2 * i + 1], SECOND);
	}
	DO_CHECK(q.size() == 10);
}
