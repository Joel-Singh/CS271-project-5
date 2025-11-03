output/main: btree.cpp btree_delete.cpp btree.h main.cpp
	# The `mkdir -p output &&` just makes a directory named output before compiling!
	mkdir -p output && g++ -o output/main main.cpp
