#include <stdio.h>
#include "rb_tree_pointer.h"

int main() {
  struct rb_root root = {.rb_node = NULL};
  insert(&root, 15);
  insert(&root, 8);
  insert(&root, 25);
  insert(&root, 2);
  insert(&root, 4);
  insert(&root, 6);
  insert(&root, 24);
  insert(&root, 23);
//  insert(&root, 2);
//  printf("color = %d, value = %d\n", root.rb_node->color, root.rb_node->value);
//  printf("color = %d, value = %d\n", root.rb_node->left->color, root.rb_node->left->value);
  print_tree(root.rb_node);
  printf("\n");
  delete(root.rb_node, 8);
  delete(root.rb_node, 24);
  delete(root.rb_node, 15);
  print_tree(root.rb_node);
  return 0;
}
