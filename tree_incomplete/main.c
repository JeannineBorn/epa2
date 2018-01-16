#include <stdbool.h>
#include "btree/btree.h"
#include "test/test.h"

void simple_insert(node_pointer node, stdelement element) {
	int size = node->number_of_elements;
	int buffer_element;
	for(int i = 0; i < size; i++) {
		if(node->elements[i] == (int) NULL) {
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
			node->elements[ORDER -1] = (int) NULL;
			simple_insert(node, element);
			return middle;
		}
	}
	middle = node->elements[ORDER];
	node->elements[ORDER] = (int) NULL;
	simple_insert(node, element);
	return middle;
}


void split(node_pointer node, int middle) {
	struct node node_r;
	struct node node_l;
	node_r.parent = node;
	node_l.parent = node;
	for(int i = ORDER; i < MAXNODE; i++){
		node_r.elements[i - ORDER] = node->elements[i];
		node_l.elements[i - ORDER] = node->elements[i - ORDER];
		node->elements[i] = (int) NULL;
		node->elements[i-ORDER] = (int) NULL;
		node_r.number_of_elements++;
		node_l.number_of_elements++;
		node->number_of_elements -= 2;
	}
	simple_insert(node, middle);
}


void insert_child(node_pointer parent, node_pointer child) {
	int size = MAXNODE +1;
	int childcount = parent->number_of_children;
	if(childcount < size){
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
			split(node, middle);
			insert_child(node->parent, node);
			return;
		}
		else{
			split(node, middle);
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

void delete (node_pointer node, stdelement element){
	if(node.number_of_children == 0){
		delete_leaf(node, element)
	}
	else {

		element =
		delete_leaf()
	}
}

void delete_leaf(node_pointer node, stdelement element){
	for(int i = 0; i < node.number_of_elements; i++){
		if(node.elements[i] == element){
			node.elements = NULL;
			node.number_of_elements --;
		}
	if(node.number_of_elements < 2){
		parent_node = node.parent; // um herauszufinden ob der rechte oder linke Knoten mehr als d Elemente hat müssen wir erstmal die Position unseres node in dem childrenarray finden
		for(int i = 0; i < parent_node.number_of_children; i++){
			if(parent_node.children[i] == node){
				int dividing_element = i;
				left_node = parent_node.children[i - 1];
				right_node = parent_node.children[i + 1];
			}
		}
		// nun kommt das checken ob left_node oder right_node mehr als d elemente hat. Annahme d=2
		if(left_node.number_of_elements > 2){
			insert_into_node(node, left_node.elements[left_node.number_of_elements -1]);
			left_node.elements[left_node.number_of_elements -1] = NULL;
			left_node.number_of_elements --; // das letzte Element vom linken Knoten wurde node beigefügt
		}
		else if (right_node.number_of_elements > 2){
			insert_into_node(node, right_node.elements[right_node.number_of_elements -1]);
			right_node.elements[right_node.number_of_elements -1] = NULL;
			right_node.number_of_elements --; // das letzte Element vom rechten Knoten wurde node beigefügt
		}
		// weder left noch right hat mehr als d elemente
		else{
			for(int i = 0; i < node.number_of_elements; i++){
				insert_into_node(rigt_node, node.elements[i]); // ich habe mich entschieden in diesem Fall den Knoten immer mit dem rechten zu verschmelzen
				node.elements[i] = NULL;
				node.number_of_elements --;
			}
			insert_into_node(right_node, dividing_element); // "übernimm das trennende Element s des Vaters v in diesen Knoten"
			node.parent[dividing_element] = NULL; // leeren Knoten abgehängt
			delete_leaf(parent_node, dividing_element);
		}
	}
	}



}

int main(void){
	int ret;

	struct node node2;
	node2.parent = NULL;
	node2.number_of_elements = 4;
	node2.elements[0] = 2;
	node2.elements[1] = 3;
	node2.elements[2] = 4;
	node2.elements[3] = 5;
	node2.children[0] = NULL;
	node2.children[1] = NULL;
	node2.children[2] = NULL;
	node2.children[3] = NULL;
	node2.children[4] = NULL;

	struct node node3;
	node3.parent = NULL;
	node3.number_of_elements = 4;
	node3.elements[0] = 1;
	node3.elements[1] = 0;
	node3.elements[2] = 1;
	node3.elements[3] = 0;
	node3.children[0] = NULL;
	node3.children[1] = NULL;
	node3.children[2] = NULL;
	node3.children[3] = NULL;
	node3.children[4] = NULL;

	struct node node1;
	node1.parent = NULL;
	node1.number_of_elements = 4;
	node1.number_of_children = 2;
	node1.elements[0] = 0;
	node1.elements[1] = 1;
	node1.elements[2] = 8;
	node1.elements[3] = 9;
	node1.children[0] = &node3;
	node1.children[1] = &node2;
	node1.children[2] = NULL;
	node1.children[3] = NULL;
	node1.children[4] = NULL;

	node2.parent = &node1;
	node3.parent = &node1;

	btree tree = create_btree();
	tree->root = &node1;

	int depth = get_btree_depth(tree);
	int width = svg_get_width(depth);
	int height = svg_get_height(depth);

	puts("render_node_to_svg_test1()");

	save_btree("render_node_to_svg_test1.tree.svg", tree);


	// FILE* fd = fopen("render_node_to_svg_test1.test.svg", "w");
	// if(fd == NULL) {
	// 	fprintf(stderr, "Error: Couldn't open file!\n");
	// 	return EXIT_FAILURE;
	// }

	// ret = svg_save_header(fd, width, height);
	// if(ret != EXIT_SUCCESS) {
	// 	fprintf(stderr, "Error: Couldn't save file!\n");
	// 	fclose(fd);
	// 	return EXIT_FAILURE;
	// }

	// ret = render_node_to_svg(fd, tree->root, 20, 30);
	// if(ret != EXIT_SUCCESS) {
	// 	fprintf(stderr, "Error: Couldn't save file!\n");
	// 	fclose(fd);
	// 	return EXIT_FAILURE;
	// }

	// ret = svg_save_footer(fd);
	// if(ret != EXIT_SUCCESS) {
	// 	fprintf(stderr, "Error: Couldn't save file!\n");
	// 	fclose(fd);
	// 	return EXIT_FAILURE;
	// }

	// fclose(fd);

	free(tree);

	//ret = assert_equals_file("render_node_to_svg_reference1.svg", "render_node_to_svg_test1.test.svg", 1024);

	return ret;

}


/*
*children counter einführen
*
*
*/