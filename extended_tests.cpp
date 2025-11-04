#include "btree_helper_functions.cpp"
#include "btree.h"
#include <string>
#include "test.cpp"

using namespace std;

void test_max_key() {
  {
    BTree tree;
    test("empty max_key returns 0", tree.max_key(tree.root), 0);
  }

  {
    BTree tree = build_tree("./tests/extended_tests/single_value.txt");
    test("max_key on single value tree", tree.max_key(tree.root), 5);

  }

  {
    BTree tree = build_tree("./tests/test_3a.txt");
    test("max_key on multiple node tree", tree.max_key(tree.root), 26);
  }

  {
    BTree tree = build_tree("./tests/test_2b.txt");
    test("max_key on multiple node tree", tree.max_key(tree.root), 30);
  }
}

void test_find_k() {
  BTree tree;
  test("empty tree returns 0", tree.find_k(tree.root, 8), 0);
}

void extended_tests() {
  test_max_key();
  test_find_k();
}
