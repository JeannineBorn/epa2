#include <stdbool.h>
#include "btree/btree.h"

void simple_insert(node_pointer node, stdelement element) {
	int size = node->number_of_elements;
	int buffer_element;
	for(int i = 0; i < size; i++) {
		if(node->elements[i] == NULL) {
			node->elements[i] = element;
		}
		if(element < node->elements[i]) {
			buffer_element = node->elements[i];
			node->elements[i] = element;
			element = buffer_element;
		}
	}
	node->elements[size] = element;
}


int middle_element(node_pointer node, stdelement element) {
	int middle;
	if(element < node->elements[ORDER]) {
		if(element > node->elements[ORDER - 1]) {
			return element;
		}
		else {
			middle = node->elements[ORDER -1];
			node->elements[ORDER -1] = NULL;
			simple_insert(node, element);
			return middle;
		}
	}
	middle = node->elements[ORDER];
	node->elements[ORDER] = NULL;
	simple_insert(node, element);
	return middle;
}


void split(node_pointer node, stdelement element, int middle) {
	struct node node_r;
	node_r.parent = node->parent;
	for(int i = ORDER; i < MAXNODE; i++){
		node_r.elements[i - ORDER] = node->elements[i];
		node->elements[i] = NULL;
		node_r.number_of_elements++;
		node->number_of_elements--;
	}
}


void insert_child(node_pointer parent, node_pointer child) {
	int size = MAXNODE +1;
	int childcount = parent->number_of_children;
	if(childcount < MAXNODE + 1){
		parent->children[childcount-1] = child;
		parent->number_of_children++;
		return;
	}
}


bool findkey(stdelement element, node_pointer current_node){
	bool success = false;
	node_pointer p = current_node;
	//struct node current_node = malloc(sizeof(struct node));
	while (!success && (p != NULL)){
		current_node = p;
		int size_array = (sizeof(current_node->elements)) / (sizeof(int));
		for(int i = 0; i < size_array; i++) {
			if (current_node->elements[i] == element) {
				success = true;
			}
			else if(current_node->elements[i] > element) {
				p = current_node->children[i];
				continue;
			}
		}
		p = current_node->children[MAXNODE];
		current_node = p;

	}
	return success;
}


bool empty(node_pointer node) {
	if(node->number_of_elements == 0) {
		return true;
	}
	return false;
}


void insert_into_node(node_pointer node, stdelement element) {
	int middle = middle_element(node, element);

	// case: the current node is full
	if(node->number_of_elements > 3) {

		// the parent node is full
		if(node->parent->children[MAXNODE] != NULL) {
			// not the simple split
			insert_into_node(node->parent, middle);
			split(node, element, middle);
			insert_child(node->parent, node);
			return;
		}
		else{
			split(node, element, middle);
			insert_child(node->parent, node);
		}
	}
	else simple_insert(node, element);
}


bool insert_element(stdelement element, btree tree){

	node_pointer current_node = tree->root;

	bool success = !findkey(element, current_node);
	if(success) {
		if (!empty(tree->root)){
			insert_into_node(current_node, element);
		}
		else {
			node_pointer node1 = malloc(sizeof(struct node));
			node1->parent = NULL;
			node1->number_of_elements = 1;
			node1->number_of_children = 0;
			node1->elements[0] = element;
			tree->root = node1;
		}
	}
	return success;
}


int main(void){
  /*insert your code below*/
    btree tree = create_btree();

  return EXIT_SUCCESS;
}


/*
*children counter einführen
*
*
*/