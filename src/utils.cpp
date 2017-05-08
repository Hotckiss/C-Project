/***************************** 
*	Author: Andrey Kirilenko, SPbAU Project
*	15.04.2017
*****************************/

#include "utils.h"
#include <iostream>

huffman_node::huffman_node() : left(NULL), right(NULL), ch(0), ch_freq(0) {}

huffman_node::huffman_node(huffman_node* left, huffman_node* right, unsigned char ch, long ch_freq) : left(left), right(right), ch(ch), ch_freq(ch_freq) {}

huffman_node::~huffman_node() {
	if(!left)
		delete left;
	if(!right)
		delete right;
}

bool compare(const huffman_node* a, const huffman_node* b) {
	if (a->ch_freq < b->ch_freq)
		return true;
	return false;
}

size_t double_queue::size() const {
	return queue_a.size() + queue_b.size();
}

void double_queue::push(huffman_node* elem, queue_number qn) {
	// push element to the first or second queue
	if (!qn)
		queue_a.push_back(elem);
	else
		queue_b.push_back(elem);
}

huffman_node* double_queue::extract_min() {
	// pop min element of the both queues, queues are sorted
	huffman_node *answer;

	// empty
	if (size() == 0)
		answer = NULL;

	// first empty
	else if (queue_a.empty()) {
		answer = queue_b.front();
		queue_b.pop_front();
	}

	// second empty
	else if (queue_b.empty()) {
		answer = queue_a.front();
		queue_a.pop_front();
	}

	// minimum from a.first, b.first
	else {
		huffman_node *huffman_node_a = queue_a.front(), *huffman_node_b = queue_b.front();
		if (huffman_node_a->ch_freq < huffman_node_b->ch_freq) {
			answer = huffman_node_a;
			queue_a.pop_front();
		}
		else {
			answer = huffman_node_b;
			queue_b.pop_front();
		}
	}
	return answer;
}

void double_queue::sort_queue() {
	std::sort(queue_a.begin(), queue_a.end(), compare);
	std::sort(queue_b.begin(), queue_b.end(), compare);
}

bit_accum::bit_accum() : accum_(0), accum_pos_(7) {}

void bit_accum::push_bit(int bit) {
	// accum is full
	if (accum_pos_ < 0)
		return;
	// add bit to the accum
	if (bit)
		accum_ |= 1 << accum_pos_;
	else
		accum_ &= ~(1 << accum_pos_);
	accum_pos_--;
}

int bit_accum::get_bit(std::ifstream &fin) {
	// if all bits were get, read next byte
	if (accum_pos_ < 0) {
		read(fin);
		accum_pos_ = 7;
	}
	// get next bit
	return ((accum_ >> accum_pos_--) & 1);
}

// prepare accum to read bits
void bit_accum::bit_read_init() {
	accum_pos_ = -1;
}

std::ofstream & bit_accum::write(std::ofstream &fout) const {
	fout.write((char*)&accum_, 1);
	return fout;
}

std::ifstream & bit_accum::read(std::ifstream &fin) {
	fin.read((char*)&accum_, 1);
	return fin;
}

bool bit_accum::is_full() const {
	return accum_pos_ < 0;
}

bool bit_accum::empty() const {
	return accum_pos_ == 7;
}

void bit_accum::clear() {
	accum_ = 0;
	accum_pos_ = 7;
}
