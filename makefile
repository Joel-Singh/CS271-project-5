main: btree.cpp btree_delete.cpp btree.h main.cpp
	# The `mkdir -p output &&` just makes a directory named output before compiling!
	mkdir -p output && g++ -o output/main main.cpp

debug: btree.cpp btree_delete.cpp btree.h main.cpp
	# -g compiles with debug symbols and -O0 disables optimization so line numbers stay consistent when using gdb!
	mkdir -p output && g++ -g -O0 -o output/main main.cpp

sanitized: btree.cpp btree_delete.cpp btree.h main.cpp
	# Adds sanitization checks, making sure we don't have any memory leaks.
	mkdir -p output && g++ -g -fsanitize=address -o output/main main.cpp
