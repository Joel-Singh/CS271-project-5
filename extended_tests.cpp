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
  BTree tree = build_tree("tests/test_4.txt");
  test("tree returns 8", tree.find_k(tree.root, 8), 0);
}


void test_min_key() {
  {
    BTree tree;
    test("empty min_key returns 0", tree.min_key(tree.root), 0);
  }

  {
    BTree tree = build_tree("./tests/extended_tests/single_value.txt");
    test("min_key on single value tree", tree.min_key(tree.root), 5);

  }

  {
    BTree tree = build_tree("./tests/test_3a.txt");
    test("min_key on multiple node tree", tree.min_key(tree.root), 3);
  }

  {
    BTree tree = build_tree("./tests/test_2b.txt");
    test("min_key on multiple node tree", tree.min_key(tree.root), 5);
  }
}

void test_remove_leaf_key() {
  BTree tree; // Creating a tree just to call remove_leaf_key
  {
    Node* node = new Node(2, true);

    node->n = 1;
    node->keys[0] = 5;

    tree.remove_leaf_key(node, 0);

    test("Removing from single key node makes it empty", node->n, 0);
  }

  {
    Node* node = new Node(2, true);
    node->n = 3;
    node->keys[0] = 1;
    node->keys[1] = 2;
    node->keys[2] = 3;

    tree.remove_leaf_key(node, 1);

    test("Removing from leaf node in the middle shifts keys", array_to_string(node->keys, node->n), "1 3");
  }

  {
    Node* node = new Node(2, true);
    node->n = 3;
    node->keys[0] = 1;
    node->keys[1] = 2;
    node->keys[2] = 3;

    tree.remove_leaf_key(node, 0);

    test("Removing from leaf node at beginning shifts keys", array_to_string(node->keys, node->n), "2 3");
  }

  {
    Node* node = new Node(2, true);
    node->n = 3;
    node->keys[0] = 1;
    node->keys[1] = 2;
    node->keys[2] = 3;

    tree.remove_leaf_key(node, 2);

    test("Removing from leaf node at end shifts keys", array_to_string(node->keys, node->n), "1 2");
  }
}

void extended_tests() {
  test_max_key();
  // test_find_k();
  test_min_key();
  test_remove_leaf_key();
}
