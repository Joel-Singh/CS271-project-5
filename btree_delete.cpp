#include "btree.h"
#include <cassert>

using namespace std;

const int DEFAULT_KEY = 0;

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b
please check for an immediate right sibling first.
*/

// delete the key k from the btree
void BTree::remove(int k) {
  remove(root, k, true);
}

// delete the key k from the btree rooted at x
void BTree::remove(Node *x, int k, bool x_root) {
  if (x == nullptr) {
    return;
  }

  // Find the first index `k` is greater than or equal to
  int precedes_k = 0;
  for (int i = 0; i < x->n; i++) {
    if (x->keys[i] >= k) {
      precedes_k = i;
      break;
    }
  }

  bool k_in_x = x->keys[precedes_k] == k;
  if (k_in_x) {
    if (x->leaf) { // Case 1: the search arrives at a leaf node x that contains k
      remove_leaf_key(x, precedes_k);
    } else { // Case 2: The search arrives at an internal node x that contains k

      bool left_has_t_keys = x->c[precedes_k]->n == t;
      if (left_has_t_keys) { // Case 2a, x.c_i has at least t keys
        int predecessor = max_key(x->c[precedes_k]);

        x->keys[precedes_k] = predecessor;
        remove(x->c[precedes_k], predecessor);
        return;
      }

      bool left_has_t_minus_one_keys = x->c[precedes_k]->n == t - 1;
      bool right_has_t_keys = x->c[precedes_k]->n == t - 1;
      if (left_has_t_minus_one_keys && right_has_t_keys) { // Case 2b
        int successor = min_key(x->c[precedes_k+1]);

        x->keys[precedes_k] = successor;
        remove(x->c[precedes_k+1], successor);
        return;
      }
    }
  }

  for (int i = 0; i < x->n + 1; i++) {
    remove(x->c[i], k);
  }
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists
int BTree::find_k(Node *x, int k) {    
    int i = 0;
    while (i < x->n && k > x->keys[i]) {
        i++;
    }

    if (i <= x->n && k == x->keys[i]) {
        return (x, i);
    } else if (x->leaf) {
        return (x->n);
    } else {
        return find_k(*x->c, k);
    }    
}

// remove the key at index i from a btree leaf node x
// What to do if the indice is invalid? Currently just asserting
void BTree::remove_leaf_key(Node *x, int i) {
  assert(x != nullptr);
  assert(x->leaf == true);

  bool i_is_valid = i >= 0 && i < x->n ;
  assert(i_is_valid);

  for (int j = i; j < (x->n - 1); j++) {
    x->keys[j] = x->keys[j+1];
  }

  x->n--;
}

// remove the key at index i and child at index j from a btree internal node x
void BTree::remove_internal_key(Node *x, int i, int j) {}

// return the max key in the btree rooted at node x
int BTree::max_key(Node *x) { 
  if (x == nullptr) {
    return DEFAULT_KEY;
  }

  if (x->leaf) {
    return x->keys[x->n - 1];
  } else {
    int rightmost_child = t*2 - 1;
    while (x->c[rightmost_child] == nullptr) {
      rightmost_child--;
    }

    return max_key(x->c[rightmost_child]);
  }
}

// return the min key in the btree rooted at node x
int BTree::min_key(Node *x) {
  if (x == nullptr) {
    return DEFAULT_KEY;
  }

  if (x->c[0] == nullptr) {
    return x->keys[0];
  } else {
    return min_key(x->c[0]);
  }
}

// merge key k and all keys and children from y into y's LEFT sibling x
void BTree::merge_left(Node *x, Node *y, int k) {}

// merge key k and all keys and children from y into y's RIGHT sibling x
void BTree::merge_right(Node *x, Node *y, int k) {}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's LEFT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_left(Node *x, Node *y, Node *z, int i) {}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_right(Node *x, Node *y, Node *z, int i) {}
