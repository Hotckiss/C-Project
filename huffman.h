/***************************** 
*	Author: Andrey Kirilenko, SPbAU Project
*	15.04.2017
*****************************/

#ifndef HUFFMAN_H_
#define HUFFMAN_H_
#include "utils.h"
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
const int buf_size = 1024;
const int ascii_size = 256;

class huffman_archiever {
public:
	huffman_archiever();
	void compress(const char *fin, const char *fout);
	void decompress(const char *fin, const char *fout);
	friend class huffman_archiever_test;
private:
	void decode_table(std::ifstream & ifs);
	void count_frequency(const char *filename);
	void make_queue();
	huffman_node* build_tree();
	void create_codes(huffman_node *n, int depth, std::string cur_code);
	void free_tree(huffman_node *elem);
	std::ofstream & encode_table(std::ofstream & ofs);
	int get_log(long num) const;
	void clear();

	double_queue q;
	bit_accum acc;                        
	std::string codes[ascii_size];
	std::vector<long> freq;
	unsigned char buffer[buf_size];
	int input_size, compressed_size, additional_size;
};

class huffman_exeption : public std::exception {
private:
	std::string _message;
public:
	huffman_exeption(const std::string &_message);
	std::string get_message() const;
	virtual ~huffman_exeption() throw() {}
};
#endif
