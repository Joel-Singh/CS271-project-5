#include "btree.h"
#include <cassert>

using namespace std;

const int DEFAULT_KEY = 0;

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b
please check for an immediate right sibling first.
*/

using namespace std;

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
  int succeeds_k = 0;
  for (; succeeds_k < x->n; succeeds_k++) {
    if (x->keys[succeeds_k] >= k) {
      break;
    }
  }

  bool k_in_x = x->keys[succeeds_k] == k;
  if (k_in_x && x->leaf) { // Case 1: the search arrives at a leaf node x that contains k
    const int k_index = succeeds_k;
    remove_leaf_key(x, k_index);
  } else if (k_in_x && (!(x->leaf))) { // Case 2: The search arrives at an internal node x that contains k
    const int k_index = succeeds_k;

    Node* left = x->c[k_index];
    Node* right = x->c[k_index + 1];

    bool left_has_t_keys = left->n == t;
    bool left_has_t_minus_one_keys = left->n == t - 1;

    bool right_has_t_keys = right->n == t;
    bool right_has_t_minus_one_keys = right->n == t - 1;

    if (left_has_t_keys) { // Case 2a, x.c_i has at least t keys
      int predecessor = max_key(left);

      x->keys[k_index] = predecessor;
      remove(left, predecessor); 
    } else if (left_has_t_minus_one_keys && right_has_t_keys) { // Case 2b
      int successor = min_key(right);

      x->keys[k_index] = successor;
      remove(right, successor);
    } else if (left_has_t_minus_one_keys && right_has_t_minus_one_keys) { // Case 2c
      // left is going to get 2t - 1 keys
      left->n = 2*t - 1;

      // Merge k and all keys in right into left
      left->keys[t - 1] = k; // Why is this so?
      for (int i = 0; i < t - 1; i++) {
        left->keys[t + i] = right->keys[i];
      }

      // Remove k from x after merge
      x->n--;
      for (int i = k_index; i < x->n; i++) {
        x->keys[i] = x->keys[i+1];
      }

      // Remove the pointer to right
      for (int i = k_index+1; i < (x->n+1); i++) {
        x->c[i] = x->c[i+1];
      }
      delete right;

      if (x_root && x->n == 0) { // Still case 2c where x is the root but becomes empty. See page 516, paragraph below case 3b.
        Node* old_root = root;
        root = x->c[0];
        root->leaf = true;
        remove(root, k);

        delete old_root;
      } else {

        // Can finally recursively delete k from left
        remove(left, k);
      }
    }
  } else if (!k_in_x && !(x->leaf)) { // Case 3
    // The subtree containing k if k is in the tree
    Node* subtree_containing_k = x->c[succeeds_k];
    if (subtree_containing_k->n == (t-1)) { // Case 3a or 3b
      Node* left_sibling = nullptr;
      Node* right_sibling = nullptr;

      if (succeeds_k < (x->n + 1)) {
        right_sibling = x->c[succeeds_k + 1];
      }

      if (succeeds_k > 0) {
        left_sibling = x->c[succeeds_k - 1];
      }

      Node* sibling_with_t_keys = nullptr;
      if (right_sibling != nullptr && right_sibling->n == t) {
        sibling_with_t_keys = right_sibling;
      } else if (left_sibling != nullptr && left_sibling->n == t) {
        sibling_with_t_keys = left_sibling;
      }

      if (((subtree_containing_k->n) == (t - 1)) && (sibling_with_t_keys != nullptr)) { // Case 3a
        if (sibling_with_t_keys == right_sibling) { // Case 3a right sibling has t keys
          swap_right(x, subtree_containing_k, right_sibling, succeeds_k);
        } else { // Case 3a left sibling has t keys
          swap_left(x, subtree_containing_k, left_sibling, succeeds_k - 1);
        }
      } else { // Both siblings must have t-1 keys
      }
    }

    remove(subtree_containing_k, k);
  }
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists
int BTree::find_k(Node *x, int k) {    
    int i = 0;
    
    if (x == nullptr) {
        return 0;
    }

    while (i < x->n && k > x->keys[i]) {
        i++;
    }

    if (i <= x->n && k == x->keys[i]) {
        return (x, i);
    } else if (x->leaf) {
        return (x->n);
    } else {
        return find_k(x->c[i], k);
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
void BTree::swap_left(Node *x, Node *y, Node *z, int i) {
  assert(x != nullptr);
  assert(y != nullptr);
  assert(z != nullptr);

  bool y_is_not_full = y->n <= (2*t - 1);
  assert(y_is_not_full);

  bool z_is_left_sibling_of_y = x->c[i] == z && x->c[i + 1] == y;
  assert(z_is_left_sibling_of_y);

  // We want to give `y` a key from it's parent `x`. Let's make some room by pushing all keys of `y` up 1.

  int succeeds_k = i + 1;
  Node* left_sibling = z;
  Node* subtree_containing_k = y;

  subtree_containing_k->n++;

  // Pushing 
  for (int i = subtree_containing_k->n - 1; i > 0; i--) {
    subtree_containing_k->keys[i] = subtree_containing_k->keys[i-1];
  }

  subtree_containing_k->keys[0] = x->keys[succeeds_k-1];

  x->keys[succeeds_k - 1] = left_sibling->keys[left_sibling->n-1];

  // Push the children of subtree up
  for (int i = subtree_containing_k->n; i > 0; i--) {
    subtree_containing_k->c[i] = subtree_containing_k->c[i-1];
  }

  subtree_containing_k->c[0] = left_sibling->c[left_sibling->n];

  left_sibling->n--;
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_right(Node *x, Node *y, Node *z, int i) {
  assert(x != nullptr);
  assert(y != nullptr);
  assert(z != nullptr);

  bool y_is_not_full = y->n <= (2*t - 1);
  assert(y_is_not_full);

  bool z_is_right_sibling_of_y = x->c[i] == y && x->c[i + 1] == z;
  assert(z_is_right_sibling_of_y);

  // Add a new key to `y`, taking the key at the dividing index.
  y->n++;
  y->keys[y->n-1] = x->keys[i];

  // The key at the dividing index is replaced by the first key in the right
  // sibling. At this point, The original key `x->keys[i]` has been fully moved
  // from x to y.
  x->keys[i] = z->keys[0];

  // Since the first key of the right sibling (`z`) is being used to replace the
  // key at the dividing index, we need to move its first child over to y.
  // Remember, y has an empty child spot as y gained a key and z has one less
  // child as z is losing a key.
  y->c[y->n] = z->c[0];

  // Move all the keys and children in the right sibling (`z`) to the left because `z` is losing its first key.
  z->n--;
  for (int i = 0; i < z->n; i++) {
    z->keys[i] = z->keys[i+1];
  }

  for (int i = 0; i < z->n + 1; i++) {
    z->c[i] = z->c[i+1];
  }
}
