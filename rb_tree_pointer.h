//
// Created by Frederick.H on 2023/9/28.
//

#ifndef RB_TREE_C__RB_TREE_POINTER_H_
#define RB_TREE_C__RB_TREE_POINTER_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define	RB_RED		0
#define	RB_BLACK	1

struct rb_node {
  int color;
  int value;
  struct rb_node *parent;
  struct rb_node *left;
  struct rb_node *right;
};

struct rb_node* new_node(int value, int color) {
  struct rb_node* node = (struct  rb_node*) malloc(sizeof(struct rb_node));
  node->value = value;
  node->color = color;
  node->left = NULL;
  node->right = NULL;
  node->parent = NULL;
  return node;
}

struct rb_root {
  struct rb_node *rb_node;
};

void rotate_right(struct rb_node* parent, struct rb_node* node) {
  struct rb_node* successor = node->left;
  node->left = successor->right;
  successor->right = node;
//  if (parent->parent == NULL) {
//    parent = successor;
//  }
  if (parent->left == node) {
    parent->left = successor;
  } else if (parent->right == node){
    parent->right = successor;
  }
  successor->parent = parent;
  node->parent = successor;
}

void rotate_left(struct rb_node* parent, struct rb_node* node) {
  struct rb_node* successor = node->right;
  node->right = successor->left;
  successor->left = node;
//  if (parent->parent == NULL) {
//    parent = successor;
//    return;
//  }
  if (parent->left == node) {
    parent->left = successor;
  } else if (parent->right == node) {
    parent->right = successor;
  }
  successor->parent = parent;
  node->parent = successor;
}

struct rb_node* sibling(struct rb_node* node) {
  if (node->parent != NULL) {
    if (node->parent->right == node) {
      return node->parent->left;
    }
    if (node->parent->left == node) {
      return node->parent->right;
    }
  }
  return NULL;
}

void maintain_after_insert(struct rb_node *node) {
  if (node->parent == NULL) {
    return;
  }
  if (node->parent->color == RB_BLACK) {
    return;
  }
  if (node->parent->parent == NULL) {
    node->parent->color = RB_BLACK;
    return;
  }

  struct rb_node* uncle = sibling(node->parent);
  if (uncle != NULL && uncle->color == RB_RED) {
    uncle->color = RB_BLACK;
    node->parent->color = RB_BLACK;
    // Has checked null before
    node->parent->parent->color = RB_RED;
    maintain_after_insert(node->parent->parent);
  } else {
    // LL, RR, LR, RL
    // RL, LR
    struct rb_node* parent = node->parent;
    if (node->parent->left == node && node->parent->parent->right == node->parent) {
      rotate_right(node->parent->parent, node->parent);
      node = parent;
    } else if (node->parent->right == node && node->parent->parent->left == node->parent) {
      rotate_left(node->parent->parent, node->parent);
      node = parent;
    }

    // LL, RR
    if (node->parent->left == node) {
      rotate_right(node->parent->parent->parent, node->parent->parent);
    } else if (node->parent->right == node){
      rotate_left(node->parent->parent->parent, node->parent->parent);
    }
    node->parent->color = RB_BLACK;
    struct rb_node* sib = sibling(node);
    sib->color = RB_RED;
    return;
  }
}

void insert_node(struct rb_node *node, int value) {
  if (value <= node->value) {
    if (node->left == NULL) {
      node->left = new_node(value, RB_RED);
      node->left->parent = node;
      maintain_after_insert(node->left);
    } else {
      insert_node(node->left, value);
    }
  } else {
    if (node->right == NULL) {
      node->right = new_node(value, RB_RED);
      node->right->parent = node;
      maintain_after_insert(node->right);
    } else {
      insert_node(node->right, value);
    }
  }
}

void insert(struct rb_root *root, int value) {
  if (root->rb_node == NULL) {
    struct rb_node* node = new_node(value, RB_BLACK);
//    root = (struct rb_root*) malloc(sizeof (struct rb_root));
    root->rb_node = node;
    return;
  }
  insert_node(root->rb_node, value);
}

void maintain_after_delete(struct rb_node* node) {
  if (node->parent == NULL) {
    return;
  }
  struct rb_node* sibling_node = sibling(node);
  if (sibling_node->color == RB_RED) {
    struct rb_node* parent = node->parent;
    if (parent->left == node) {
      rotate_left(node->parent->parent,node->parent);
    } else if (parent->right == node) {
      rotate_right(node->parent->parent, node->parent);
    }
    sibling_node->color = RB_BLACK;
    parent->color = RB_RED;
    sibling_node = sibling(node);
  }

  struct rb_node *closed_nephew = NULL, *distant_nephew = NULL;
  if (node->parent->left == node) {
    closed_nephew = sibling_node->left;
    distant_nephew = sibling_node->right;
  } else {
    closed_nephew = sibling_node->right;
    distant_nephew = sibling_node->left;
  }
  bool closed_nephew_black = closed_nephew == NULL || closed_nephew->color == RB_BLACK;
  bool distant_nephew_black = distant_nephew == NULL || distant_nephew->color == RB_BLACK;

  if (closed_nephew_black && distant_nephew_black) {
    if (node->parent->color == RB_RED) {
      sibling_node->color = RB_RED;
      node->parent->color = RB_BLACK;
      return;
    } else {
      sibling_node->color = RB_RED;
      maintain_after_delete(node->parent);
      return;
    }
  } else {
    if (closed_nephew != NULL && closed_nephew->color == RB_RED) {
      if (node->parent->right == node) {
        rotate_left(sibling_node->parent, sibling_node);
      } else if (node->parent->left == node) {
        rotate_right(sibling_node->parent, sibling_node);
      }
      closed_nephew->color = RB_BLACK;
      sibling_node->color = RB_RED;
      sibling_node = sibling(node);
      if (node->parent->left == node) {
        closed_nephew = sibling_node->left;
        distant_nephew = sibling_node->right;
      } else {
        closed_nephew = sibling_node->right;
        distant_nephew = sibling_node->left;
      }
      if (node->parent->left == node) {
        rotate_left(node->parent->parent, node->parent);
      } else if (node->parent->right == node) {
        rotate_right(node->parent->parent, node->parent);
      }
      sibling_node->color = node->parent->color;
      node->parent->color = RB_BLACK;
      if (distant_nephew != NULL) {
        distant_nephew->color = RB_BLACK;
      }
    }
  }
}

bool delete(struct rb_node *node, int value) {
  if (value < node->value) {
    if (node->left != NULL) {
      return delete(node->left, value);
    } else {
      return false;
    }
  } else if (value > node->value) {
    if (node->right != NULL) {
      return delete(node->right, value);
    } else {
      return false;
    }
  }

  if (node->parent == NULL && node->left == NULL && node->right == NULL) {
    free(node);
    node = NULL;
    return true;
  }

  if (node->left != NULL && node->right != NULL) {
    struct rb_node* successor = node->right;
    struct rb_node* parent = node;
    while (successor->left != NULL) {
      parent = successor;
      successor = parent->left;
    }
    // swap
    int tmp = node->value;
    node->value = successor->value;
    successor->value = tmp;
    struct rb_node* tmp_ptr = node;
    node = successor;
//    successor = tmp_ptr;
  }

  if (node->left == NULL && node->right == NULL) {
    if (node->color == RB_BLACK) {
      maintain_after_delete(node);
    }
    struct rb_node* parent = node->parent;
    if (parent->left == node) {
      parent->left = NULL;
    } else if (parent->right == node) {
      parent->right = NULL;
    }
    free(node);
  } else {
    if (node->parent == NULL) {
      return false;
    }
    struct rb_node* parent = node->parent;
    struct rb_node* replacement = node->left != NULL ? node->left : node->right;
    if (parent->left == node) {
      parent->left = replacement;
    } else if (parent->right == node) {
      parent->right = replacement;
    }
    replacement->parent = parent;
    if (node->color == RB_BLACK) {
      if (replacement->color == RB_RED) {
        replacement->color = RB_BLACK;
      } else {
        maintain_after_delete(replacement);
      }
    }
    free(node);
  }
  return true;
}

struct queue {
  int start;
  int end;
  struct rb_node* arr[16];
};

void push(struct queue* queue, struct rb_node* node) {
  queue->arr[queue->end] = node;
  queue->end = (queue->end + 1) % 16;
}

struct rb_node* pop(struct queue* queue) {
  if (queue->start == queue->end) {
    return NULL;
  }
  struct rb_node* node = queue->arr[queue->start];
  queue->start = (queue->start + 1) % 16;
  return node;
}

bool is_empty(struct queue* queue) {
  if (queue->start == queue->end) {
    return true;
  }
  return false;
}

void print_tree(struct rb_node *node) {
  struct queue q = {.start = 0, .end = 0};
  push(&q, node);
  while (!is_empty(&q)) {
    int l = q.end - q.start;
    for (int i = 0; i < l; i++) {
      struct rb_node* node = pop(&q);
      printf("color = %d value = %d\t", node->color, node->value);
      if (node->left != NULL) {
        push(&q, node->left);
      }
      if (node->right != NULL) {
        push(&q, node->right);
      }
    }
    printf("\n");
  }
}

#endif //RB_TREE_C__RB_TREE_POINTER_H_
