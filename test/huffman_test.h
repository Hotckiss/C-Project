#ifndef HUFFMAN_TEST_H_
#define HUFFMAN_TEST_H_

#include <cstdio>
#include <iostream>

#define DO_CHECK(EXPR) (check((EXPR), (__FUNCTION__), (__FILE__), (__LINE__)))
class huffman_test {
protected:
	static int failed_num;
	static int total_num;
public:
	static void check(bool expr, const char *func, const char  *filename, size_t lineNum);
	static void show_final_result();
	virtual void run_all_tests() = 0;
};
#endif
