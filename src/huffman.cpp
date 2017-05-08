/***************************** 
*	Author: Andrey Kirilenko, SPbAU Project
*	15.04.2017
*****************************/

#include "huffman.h"
#include <ctime>

huffman_exeption::huffman_exeption(const std::string &_message) : _message(_message) {}

std::string huffman_exeption::get_message() const {
		return _message;
}

huffman_archiever::huffman_archiever() : input_size(0), compressed_size(0), additional_size(0) {
	freq.resize(ascii_size);
}

void huffman_archiever::compress(const char *fin, const char *fout) {
	count_frequency(fin);
	make_queue();
	huffman_node* root = build_tree();
	create_codes(root, 0, "");

	std::ifstream ifs(fin, std::ifstream::binary);
	if (ifs.is_open() == false) {
		throw huffman_exeption("Failed opening input file.");
	}

	std::ofstream ofs(fout, std::ofstream::binary);
	encode_table(ofs);
	// read a buffer of characters and code it
	do {
		ifs.read((char*)buffer, sizeof(buffer));
		for (size_t i = 0; i < (size_t)ifs.gcount(); ++i) {
			for (size_t j = 0; j < (size_t)codes[buffer[i]].length(); ++j) {
				// write 8 bits
				if (acc.is_full()) {
					acc.write(ofs);
					acc.clear();
					compressed_size++;
				}
				acc.push_bit(codes[buffer[i]][j] - '0');
			}
		}	
	} while (ifs.gcount() > 0);
	// if less than 8 bits in accum, write it immidiately
	if (!acc.empty()) {
		acc.write(ofs);
		compressed_size++;
	}
	acc.clear();
	// free memory
	free_tree(root);
	std::cerr << input_size << std::endl << compressed_size << std::endl << additional_size << std::endl;
	clear();
}

void huffman_archiever::decompress(const char *fin, const char *fout) {
	std::ifstream ifs(fin, std::ifstream::binary);
	acc.bit_read_init();

	if (ifs.is_open() == false) {
		throw huffman_exeption("Failed opening input file.");
	}

	std::ofstream ofs(fout, std::ofstream::binary);
	decode_table(ifs);

	make_queue();
	huffman_node *root = build_tree(), *cur_huffman_node = root;
	create_codes(root, 0, "");
	// empty file was compressed, empty tree
	if (root == NULL) {
		std::cerr << compressed_size << "\n" << input_size << "\n" << additional_size << "\n";
		return;
	}
	// file contains one type of character, but depth of the tree must be non-zero
	if (root->left == NULL) {
		root->right = new huffman_node(NULL, NULL, root->ch, root->ch_freq);
		root->left = new huffman_node(NULL, NULL, root->ch, 0);
	}

	for (int i = 0; i < ascii_size; ++i)
		compressed_size += freq[i] * codes[i].length();

	compressed_size = (compressed_size + 7) / 8;
	long n = root->ch_freq;
	input_size = n;
	acc.bit_read_init();
	// sift dowh to the leaf and get character
	while (n > 0) {
		int bit = acc.get_bit(ifs);
		if (cur_huffman_node->left == NULL) {
			ofs.write((char*)&cur_huffman_node->ch, 1);
			cur_huffman_node = root;
			n--;
		}
		if (bit)
			cur_huffman_node = cur_huffman_node->right;
		else
			cur_huffman_node = cur_huffman_node->left;
	}
	free_tree(root);
	std::cerr << compressed_size << std::endl << input_size << std::endl << additional_size << std::endl;
	clear();
}

void huffman_archiever::decode_table(std::ifstream & ifs) {
	acc.bit_read_init();
	int freq_len = 0, current_bit = 0, current_freq = 0;
	ifs.read((char*)&freq_len, 1);
	additional_size = 1 + (freq_len * ascii_size + 7) / 8;
	for (int i = 0; i < ascii_size; ++i) {
		for (int j = 0; j < freq_len; ++j) {
			int bit = acc.get_bit(ifs);
			current_freq += (bit << current_bit);
			current_bit++;
		}
			freq[i] = current_freq;
			current_bit = 0;
			current_freq = 0;
	}
	acc.clear();
}

// write table, using 32 + log2(MAX_FREQ) bits
std::ofstream & huffman_archiever::encode_table(std::ofstream & ofs) {
	long max_len = *max_element(freq.begin(), freq.end());
	int freq_len = get_log(max_len + 1);
	ofs.write((char*)&freq_len, 1);
	additional_size += 1;
	for (int i = 0; i < ascii_size; ++i)
		for (int j = 0; j < freq_len; ++j) {
			if (acc.is_full()) {
				acc.write(ofs);
				acc.clear();
				additional_size++;
			}
			acc.push_bit((freq[i] >> j) & 1);
		}
	if (!acc.empty()) {
		acc.write(ofs);
		additional_size++;
	}
	acc.clear();
	return ofs;
}

void huffman_archiever::count_frequency(const char *filename) {
	fill(freq.begin(), freq.end(), 0);
	std::ifstream ifs(filename, std::ifstream::binary);

	if (ifs.is_open() == false) {
		throw huffman_exeption("Failed opening input file.");
	}

	do {
		ifs.read((char*)buffer, sizeof(buffer));
		input_size += (int)ifs.gcount();
		for (int i = 0; i < ifs.gcount(); ++i)
			freq[buffer[i]]++;
	} while (ifs.gcount() > 0);

}

// push all character nodes to the first queue and sort it by frequency
void huffman_archiever::make_queue() {
	for (int i = 0; i < ascii_size; i++)
		if (freq[i])
			q.push(new huffman_node(NULL, NULL, i, freq[i]), FIRST);
	q.sort_queue();
}

// build tree, using two queues
huffman_node* huffman_archiever::build_tree() {
	while (q.size() > 1) {
		huffman_node *a = q.extract_min(), *b = q.extract_min();
		q.push(new huffman_node(a, b, 0, a->ch_freq + b->ch_freq), SECOND);
	}
	return q.extract_min();
}

// std::string more than x5 faster, than std::vector<bool>
void huffman_archiever::create_codes(huffman_node *n, int depth, std::string cur_code) {
	if (n == NULL)
		return;
	if (n->left == NULL) {
		codes[n->ch] = cur_code;
		// if file contains only one type of character, code must not be empty
		if (!depth)
			codes[n->ch] += "0";
	}
	else {
		create_codes(n->left, depth + 1, cur_code + "0");
		create_codes(n->right, depth + 1, cur_code + "1");
	}
}

void huffman_archiever::free_tree(huffman_node *elem) {
	// dfs
	if (!elem)
		return;
	free_tree(elem->left);
	free_tree(elem->right);
	delete elem;
	elem = NULL;
}

int huffman_archiever::get_log(long num) const {
	int pow_ = 0;
	long exp_ = 1;
	while (exp_ < num)
		exp_ <<= 1, pow_++;
	return pow_;
}

void huffman_archiever::clear() {
	input_size = 0;
	compressed_size = 0;
	additional_size = 0;
}
