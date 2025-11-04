#include "btree.h"

#ifndef BTREE_HELPER_FUNCTIONS
#define BTREE_HELPER_FUNCTIONS
// Helper: build tree from file
BTree build_tree(std::string fname) { return BTree(fname); }

// Helper: capture tree print into a string
std::string tree_str(BTree &tree) {
  std::ostringstream out;
  std::streambuf *oldBuf = std::cout.rdbuf(out.rdbuf());
  tree.print();
  std::cout.rdbuf(oldBuf);
  return out.str();
}

// Helper: capture file into a string
std::string get_result(std::string fname) {
  std::ifstream in(fname);
  if (!in) {
    std::cerr << "Error opening file\n";
    return "error";
  }

  std::string result_str((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
  return result_str;
}

// Helper: vaidate result
void check_result(std::string result, std::string res_file, std::string msg,
                  int &correct_count) {
  BTree tree(res_file);
  std::string expected = tree_str(tree);
  if (result == expected) {
    correct_count += 1;
  } else {
    std::cout << msg << std::endl;
    std::cout << "\texpected tree:" << std::endl << expected << std::endl;
    std::cout << "\treceived tree:" << std::endl << result << std::endl;
  }
}
#endif
