/***************************** 
*	Author: Andrey Kirilenko, SPbAU Project
*	15.04.2017
*****************************/

#include "huffman.h"
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
	std::ios::sync_with_stdio(false);
	huffman_archiever ha;
	if(argc == 2) {
		if(!strcmp(argv[1], "-h"))
			cerr << "Input file: -f(--file) + filename\nOutput file: -o(--output) + filename\nCompression: -c; decompression -u(should be correct input file)\n";
		else
			return 0;
	}
	if(argc != 6)
		return 0;
	int code_or_decode = -1;
	for(int i = 1; i < 6; ++i)
		if(!strcmp("-c", argv[i]))
			code_or_decode = 0;
		else if(!strcmp("-u", argv[i]))
			code_or_decode = 1;
	string fin, fout;
	for(int i = 1; i < 6; ++i)
		if(!strcmp("-f", argv[i]) || !strcmp("--file", argv[i]))
			fin = argv[i + 1];
		else if(!strcmp("-o", argv[i]) || !strcmp("--output", argv[i]))
			fout = argv[i + 1];
	try {
		if(code_or_decode == 0)
			ha.compress(fin.c_str(), fout.c_str());
		else if(code_or_decode == 1)
			ha.decompress(fin.c_str(), fout.c_str());
		else
			throw huffman_exeption("Invalid arguments.");
	}
	catch(const huffman_exeption & he) {
		cerr << he.get_message();
	}
	return 0;
}
