#ifndef __BTREE_H__
#define __BTREE_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "btree_config.h"
#include "svg.h"

/*
 * pointer to a node struct
 */
typedef struct node * node_pointer;

/*
 * use unsigned integers as stdelement to make output possible
 */
typedef unsigned int stdelement;

/*
 * struct for the b-tree data structure
 */
struct node {
  node_pointer parent;
  short number_of_elements;
  short number_of_children;
  stdelement elements[MAXNODE];
  node_pointer children[MAXNODE + 1];
};

struct node_element{
	node_pointer node;
	short index;
};

struct compare_result {
    int is_equal;
    enum {DEPTH, NUMBER_OF_ELEMENTS, ELEMENT} difference;
    int depth;
    int *path;
};

typedef struct {
    struct node *root;
} * btree;

btree create_btree();
void destroy_btree(btree tree);

int get_btree_depth(btree tree);
int render_node_to_svg(FILE* fd, node_pointer node, int x, int y);
int save_btree_part(FILE* fd, node_pointer tree, int x, int y, int prev_stride);
int save_btree(const char* path, btree tree);


int compare(btree tree_a, btree tree_b, struct compare_result *result);
void print_compare(btree tree_a, btree tree_b);

#endif //__BTREE_H__
