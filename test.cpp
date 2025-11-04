#include <iostream>
#include "btree.h"
#include "btree_helper_functions.cpp"

using namespace std;

#ifndef JOEL_TEST_FUNCTION
#define JOEL_TEST_FUNCTION

//=================================================
// stringify
// Create a string from a string, number, or type implementing to_string
//
// Parameters:
//  value: value to convert
//=================================================
template <typename T> static string stringify(T &value) {
  if constexpr (std::is_same_v<T, bool>) {
    if (value) {
      return "true";
    } else {
      return "false";
    }
  } else if constexpr (std::is_same_v<T, char>) {
    if (value == '\0') { // If the character is the zero character, return an
                         // empty string directly. The string constructor would
                         // instead create a non-empty string that contains
                         // '\0'.
      return "";
    } else {
      return std::string(1, value);
    }
  } else if constexpr (std::is_arithmetic_v<T>) {
    return std::to_string(value);
  } else if constexpr (std::is_same_v<T, std::string>) {
    return value;
  } else if constexpr (std::is_same_v<T, BTree>) {
    return tree_str(value);
  } else {
    return value.to_string();
  }
}

//=================================================
// test
// Tests if an `actual` value is actually `expected`, printing appropriately.
// Will convert `actual` to a string, make sure that `expected` is the converted
// string you expect.
//
// Example:
// LinkedList myLinkedList;
// myLinkedList.push(1);
// myLinkedList.push(2);
// myLinkedList.push(3);
//
// test("push method on linked list", myLinkedList, "1 2 3");
//
// The example would output:
// > Testing `push method on linked list`
// > Successful
//
// PARAMETERS:
//  description: The description of the test
//  actual: The actual value
//  expected: What you expect `actual` value is
//=================================================
template <typename T, typename D>
void test(std::string description, T actual, const D expected) {
  cout << "Testing `" << description << "`" << endl;

  if (stringify(actual) != stringify(expected)) {
    cout << "FAILED" << endl;
    cout << "Expected: `" << stringify(expected) << "`" << endl;
    cout << "Actual: `" << stringify(actual) << "`" << endl;

  } else {
    cout << "Successful" << endl;
  }
  cout << "------------------------------" << endl;
}

//=================================================
// array_to_string
// Convert an array to a string representation
//
// PARAMETERS:
//  A: Array to convert
//  len: Length of `A`
//
// RETURN VALUE:
//  string representation of `A`
//=================================================
template <typename T> static string array_to_string(T *A, int len) {
  if (len == 0) {
    return "";
  } else {
    string str = to_string(A[0]);

    for (int i = 1; i < len; i++) {
      str += (" " + to_string(A[i]));
    }

    return str;
  }
}

#endif
