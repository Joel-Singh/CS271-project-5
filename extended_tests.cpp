#include "btree_helper_functions.cpp"
#include "btree.h"
#include <string>
#include "test.cpp"

using namespace std;

const string test_path = "./tests/extended_tests/";

void test_max_key() {
  BTree tree;
  test("empty max_key returns 0", tree.max_key(tree.root), 0);
}

void test_find_k() {
  BTree tree;
  test("empty tree returns 0", tree.find_k(tree.root, 8), 0);
}

void extended_tests() {
  test_max_key();
  test_find_k();
}
