/***************************** 
*	Author: Andrey Kirilenko, SPbAU Project
*	15.04.2017
*****************************/

#ifndef UTILS_H_
#define UTILS_H_
#include <deque>
#include <cstdlib>
#include <fstream>
#include <algorithm>

// huffman tree node class
class huffman_node {
public:
	huffman_node();
	huffman_node(huffman_node* left, huffman_node* right, unsigned char ch, long ch_freq);
	~huffman_node();
	huffman_node *left, *right; // tree structure
	unsigned char ch;           // symbol
	long ch_freq;               // frequency of the charcter
};

enum queue_number {FIRST, SECOND};

// class that contains two queues to built huffman tree
class double_queue {
public:
	size_t size() const;
	void push(huffman_node* elem, queue_number qn);
	huffman_node* extract_min();
	void sort_queue();
private:
	std::deque<huffman_node*> queue_a, queue_b;
};

// class to read and write bits from the file
class bit_accum {
public:
	bit_accum();
	void push_bit(int bit);
	void bit_read_init(); 
	int get_bit(std::ifstream &fin);
	std::ofstream & write(std::ofstream &fout) const;
	std::ifstream & read(std::ifstream &fin);
	bool is_full() const;
	bool empty() const;
	void clear();
private:
	unsigned char accum_;
	char accum_pos_;
};
#endif
