all: bin main
test: bin test1
bin:
	mkdir -p bin
main:bin/main.o bin/huffman.o bin/utils.o
	g++ -Wall -Werror bin/main.o bin/huffman.o bin/utils.o -o huffman
test1:bin/test.o bin/huffman_test.o bin/autotest.o bin/huffman.o bin/utils.o
	g++ -Wall -Werror bin/test.o bin/huffman_test.o bin/autotest.o bin/huffman.o bin/utils.o -o test_main
bin/main.o:src/main.cpp src/huffman.h
	g++ -Wall -Werror -c src/main.cpp -o bin/main.o
bin/huffman.o:src/huffman.cpp src/huffman.h src/utils.h
	g++ -Wall -Werror -c src/huffman.cpp -o bin/huffman.o
bin/utils.o:src/utils.cpp src/utils.h
	g++ -Wall -Werror -c src/utils.cpp -o bin/utils.o
bin/huffman_test.o:test/huffman_test.cpp test/huffman_test.h
	g++ -Wall -Werror -c test/huffman_test.cpp -o bin/huffman_test.o
bin/autotest.o:test/autotest.cpp test/autotest.h test/huffman_test.h src/huffman.h
	g++ -Wall -Werror -c test/autotest.cpp -o bin/autotest.o
bin/test.o:test/test.cpp test/autotest.h
	g++ -Wall -Werror -c test/test.cpp -o bin/test.o
clean:
	rm -rf bin huffman test_main
