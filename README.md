# C-Project
Проект архиватор на С++
Проект сжимет файлы алгоритмом Хаффмана, содержит тесты.
Собирается командой make. Появляется исполняемый файл huffman, сжатие задается ключом -c, расжатие ключом -u. Имя входного файла задается ключом -f(--file), выходной -o(--output) на экран выводится размер входного и выходного файла, а так же размер дополнительных данных(в них входит размер таблицы встречаемостей). Пример: ./huffman -c -f test.txt -o out; ./huffman -u --file out --output answer.txt;
Имеются тесты, собирать командой make test, запуск: ./test_main
make clean удаляет объектные и исполняемые файлы из директории.
