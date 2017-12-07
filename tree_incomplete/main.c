#include "btree/btree.h"

void simple_insert(node n, stdelement element) {
	int size = n.number_of_elements;
	int buffer_element;
	for(int i = 0; i < size; i++) {
		if(n.elements[i] == NULL) {
			n.elements[i] = element;
		}
		if(element < n.elements[i]) {
			buffer_element = n.elements[i];
			n.elements[i] = element;
			element = buffer_element;
		}
	}
	n.elements[size] = element;
}


int middle_element(node n, stdelement element) {
	int middle;
	if(element < n.elements[ORDER]) {
		if(element > n.elements[ORDER - 1]) {
			return element;
		}
		else {
			middle = n.elements[ORDER -1];
			n.elements[ORDER -1] = NULL;
			simple_insert(n,element);
			return middle;
		}
	}
	middle = n.elements[ORDER];
	n.elements[ORDER] = NULL;
	simple_insert(n,element);
	return middle;
}


void split(node n, stdelement element, int middle) {
	struct node node_r;
	node_r.parent = n.parent;
	for(int i = ORDER; i < MAXNODE; i++){
		node_r.elements[i - ORDER] = n.elements[i];
		n.elements[i] = NULL;
		node_r.number_of_elements++;
		n.number_of_elements--;
	}
}


void insert_child(node parent, node child) {
	node_pointer pchild = child;
	int size = MAXNODE +1;
	int childcount = parent.number_of_children;
	if(childcount < MAXNODE + 1){
		parent.children[childcount-1] = pchild;
		parent.number_of_children++;
		return;
	}
}


bool insert_element(stdelement element){

	struct node *current_node = btree->root;

	bool success = !findkey(element, current_node);
	if(success) {
		if (!empty(btree)){
			insert_into_node(current_node, element);
		}
		else {
			struct node node1 = malloc(sizeof(struct node));
			node1.parent = NULL;
			node1.number_of_elements = 1;
			node1.number_of_children = 0;
			node1.elements[0] = element;
			btree->root = node1;
		}
	}
	return success;
}


bool findkey(stdelement element, node current_node){
	bool success = false;
	struct node p = *btree->root;
	//struct node current_node = malloc(sizeof(struct node));
	while (!success && (p != NULL)){
		current_node = p;
		int size_array = (sizeof(current_node.elements)) / (sizeof(int));
		for(int i = 0; i < size_array; i++) {
			if (current_node.elements[i] == element) {
				success = true;
			}
			else if(current_node.elements[i] > element) {
				p = *(current_node.children[i]);
				continue;
			}
		}
		p = *(current_node.children[MAXNODE]);
		current_node = p;

	}
	return success;
}


void insert_into_node(node n, stdelement element) {
	int middle = middle_element(n, element);

	// case: the current node is full
	if(n.number_of_elements > 3) {

		// the parent node is full
		if(n.parent->children[MAXNODE] != NULL) {

			// not the simple split
			insert_into_node(n.parent, middle);
			split(n, element, middle);
			insert_child(n.parent, n);
			return;
		}
		else{
			split(n, element, middle);
			insert_child(n.parent, n);
		}
	}
	else simple_insert(n,element);

}


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