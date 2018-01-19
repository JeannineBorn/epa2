#include <stdbool.h>
#include "btree/btree.h"
#include "test/test.h"

void simple_insert(node_pointer node, stdelement element) {
	printf("simple_insert\n");
	int size = node->number_of_elements;
	int buffer_element;
	printf("size: %d", size);
	if(size < 2 * ORDER){
		for(int i = 0; i < size; i++) {
			printf("s_iter: %d\n", i);
			if(node->elements[i] == (int) NULL) {
				printf("s_found\n");
				node->elements[i] = element;
				node->number_of_elements++;
				return;
			}
			if(element < node->elements[i]) {
				printf("s_swapped\n");
				buffer_element = node->elements[i];
				node->elements[i] = element;
				element = buffer_element;
			}
		}
		printf("s_last\n");
		node->elements[size] = element;
		node->number_of_elements++;
	}
}


int middle_element(node_pointer node, stdelement element) {
	printf("middle_element\n");
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


void insert_child(node_pointer parent, node_pointer child) {
	printf("insert_child\n");
	int size = MAXNODE +1;
	int childcount = parent->number_of_children;
	int amount_p_elements = parent->number_of_elements;
	int index;

	if(childcount < size){
		stdelement first_elem = child->elements[0];
		for(int i = 0; i < amount_p_elements; i++) {
			if(parent->elements[i] > first_elem){
				parent->children[i] = child;
				parent->number_of_children++;
				child->parent = parent;
				return;
			}
			index = i;
		}

		parent->children[index + 1] = child;
		parent->number_of_children++;
		child->parent = parent;
	}
}

void insert_into_node(node_pointer, stdelement, btree);

void split2(node_pointer node, stdelement middle, btree tree) {
	printf("split\n");
	 //printf("s: %d", (sizeof(struct node)));
	 //printf(malloc((sizeof(struct node))));
	malloc((sizeof(struct node)));
	int node_size = sizeof(struct node);
	node_pointer rig = malloc(node_size);
	for(int i = 0; i < MAXNODE; i++){
		rig->elements[i] = NULL;
	}
	rig->number_of_elements = 0;
	rig->number_of_children = 0;
	

	for(int i = ORDER; i < MAXNODE; i++) {
		simple_insert(rig, node->elements[i]);

		node->elements[i] = NULL;
		node->number_of_elements--;
	}

	//TODO: childs umhängen
	//TODO: not done yet!!!!
	node_pointer root_new = tree->root;
	node_pointer root_old = tree->root;

	if(node->parent == NULL){
		// neuen root erstellen
		root_new = malloc(sizeof(struct node));
		root_new->number_of_children = 0;
		root_new->number_of_elements = 0;

		for(int i = 0; i < MAXNODE; i++){
			root_new->elements[i] = NULL;
			root_new->children[i] = NULL;
		}
		tree->root = root_new;
	}

	insert_into_node(root_new, middle, tree);
	insert_child(root_new, rig);
	insert_child(root_new, root_old);
}

bool is_leaf(node_pointer node) {
	if(node->number_of_children == 0){
		return true;
	}
	return false;
}


void split(node_pointer parent, int position, node_pointer child) {
	int node_size = sizeof(struct node);
	node_pointer right = malloc(node_size);

	for(int i = 0; i < MAXNODE; i++){
		right->elements[i] = NULL;
	}
	right->number_of_elements = 0;
	right->number_of_children = 0;

}


bool findkey(stdelement element, node_pointer *current_node){
	printf("findkey\n");
	bool success = false;
	// printf("elem: %d\n", element);
	// printf("elems-> %d\n", current_node->elements[0]);
	node_pointer target = *current_node;
	int size_array = target->number_of_elements;
	// printf("array size: %d\n", size_array);
	for(int i = 0; i < size_array; i++) {
		if(element >= target->elements[i]) {
			// printf("elem in array: %d\n",target->elements[i] );
			// printf("element: %d\n", element);
			// printf("check %d\n", (element == target->elements[i]));
			if(element == target->elements[i]) {
				*current_node = target;
				printf("found_n: %p\n", *current_node);
				return success = true;
			}
		}
		else {
			*current_node = target->children[i];
			// printf("found_n: %d\n", current_node);
			return success = findkey(element, current_node);	
		}
	}
	printf("found_n_: %p\n", *current_node);
	printf("success: %d", success);
	return success;
}


bool contains(node_pointer node, stdelement element) {
	bool success = false;
	int size = node->number_of_elements;
	for(int i = 0; i < size; i++) {
		if(node->elements[i] == element) {
			return !success;
		}
	}
	return success;
}


bool empty(node_pointer node) {
	printf("empty\n");
	if(node->number_of_elements == 0) {
		return true;
	}
	return false;
}


void insert_into_node(node_pointer current, stdelement element, btree tree) {
	printf("insert_into_node\n");
	printf("current_e: %d\n", current->number_of_elements);
	printf("i_node: %p\n", current);

	if(current == NULL){
		// tree->;
	}

	if(current->number_of_elements < MAXNODE) {
	printf("insert_into_node_simple_i\n");
		simple_insert(current, element);
		return;
	}
	split(current, (middle_element(current, element)), tree);
}


bool insert_element(stdelement element, btree tree){
	printf("insert_element\n");
	node_pointer current_node = tree->root;

	bool success = !findkey(element, &current_node);
	if(success) {
		if (!empty(tree->root)){
			insert_into_node(current_node, element, tree);
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


// void delete_leaf(node_pointer node, stdelement element){
// 	for(int i = 0; i < node->number_of_elements; i++){
// 		if(node->elements[i] == element){
// 			node->elements[i] = NULL;
// 			node->number_of_elements --;
// 		}
// 	if(node->number_of_elements < 2){
// 		node_pointer parent_node = node->parent; // um herauszufinden ob der rechte oder linke Knoten mehr als d Elemente hat müssen wir erstmal die Position unseres node in dem childrenarray finden
// 		node_pointer left_node;
// 		node_pointer right_node;
// 		int dividing_element;

// 		for(int i = 0; i < parent_node->number_of_children; i++){
// 			if(parent_node->children[i] == node){
// 				dividing_element = i;
// 				left_node = parent_node->children[i - 1];
// 				right_node = parent_node->children[i + 1];
// 			}
// 		}
// 		// nun kommt das checken ob left_node oder right_node mehr als d elemente hat. Annahme d=2
// 		if(left_node->number_of_elements > 2){
// 			insert_into_node(node, left_node->elements[left_node->number_of_elements -1]);
// 			left_node->elements[left_node->number_of_elements -1] = NULL;
// 			left_node->number_of_elements --; // das letzte Element vom linken Knoten wurde node beigefügt
// 		}
// 		else if (right_node->number_of_elements > 2){
// 			insert_into_node(node, right_node->elements[right_node->number_of_elements -1]);
// 			right_node->elements[right_node->number_of_elements -1] = NULL;
// 			right_node->number_of_elements --; // das letzte Element vom rechten Knoten wurde node beigefügt
// 		}
// 		// weder left noch right hat mehr als d elemente
// 		else{
// 			for(int i = 0; i < node->number_of_elements; i++){
// 				insert_into_node(right_node, node->elements[i]); // ich habe mich entschieden in diesem Fall den Knoten immer mit dem rechten zu verschmelzen
// 				node->elements[i] = NULL;
// 				node->number_of_elements --;
// 			}
// 			insert_into_node(right_node, dividing_element); // "übernimm das trennende Element s des Vaters v in diesen Knoten"
// 			//TODO: node->parent[dividing_element] = NULL; // leeren Knoten abgehängt
// 			delete_leaf(parent_node, dividing_element);
// 		}
// 	}
// 	}
// }

// void delete (node_pointer node, stdelement element){
// 	if(node->number_of_children == 0){
// 		delete_leaf(node, element);
// 	}
// 	else {
// 		//element =
// 		//delete_leaf();
// 	}
// }

int main(void){
	int ret;

	// struct node node2;
	// node2.parent = NULL;
	// node2.number_of_elements = 2;
	// node2.elements[0] = 1;
	// node2.elements[1] = 2;
	// node2.elements[2] = NULL;
	// node2.elements[3] = NULL;
	// node2.children[0] = NULL;
	// node2.children[1] = NULL;
	// node2.children[2] = NULL;
	// node2.children[3] = NULL;
	// node2.children[4] = NULL;

	// struct node node3;
	// node3.parent = NULL;
	// node3.number_of_elements = 2;
	// node3.elements[0] = 8;
	// node3.elements[1] = 10;
	// node3.elements[2] = NULL;
	// node3.elements[3] = NULL;
	// node3.children[0] = NULL;
	// node3.children[1] = NULL;
	// node3.children[2] = NULL;
	// node3.children[3] = NULL;
	// node3.children[4] = NULL;

	// struct node node4;
	// node4.parent = NULL;
	// node4.number_of_elements = 4;
	// node4.elements[0] = 30;
	// node4.elements[1] = 40;
	// node4.elements[2] = 50;
	// node4.elements[3] = 60;
	// node4.children[0] = NULL;
	// node4.children[1] = NULL;
	// node4.children[2] = NULL;
	// node4.children[3] = NULL;
	// node4.children[4] = NULL;

	struct node node1;
	node1.parent = NULL;
	node1.number_of_elements = 0;
	node1.number_of_children = 0;
	node1.elements[0] = NULL;
	node1.elements[1] = NULL;
	node1.elements[2] = NULL;
	node1.elements[3] = NULL;
	node1.children[0] = NULL; //&node2;
	node1.children[1] = NULL;
	node1.children[2] = NULL; //&node3;
	node1.children[3] = NULL;
	node1.children[4] = NULL; //&node4;

	// node2.parent = &node1;
	// node3.parent = &node1;
	// node4.parent = &node1;

	btree tree = create_btree();
	tree->root = &node1;

	stdelement elem0 = 30;
	stdelement elem1 = 38;
	stdelement elem2 = 42;
	stdelement elem3 = 10;

	node_pointer np = &node1;
	// node_pointer ch = &node4;
	// node_pointer n3 = &node3;


	printf("parent: %p\n", np->parent);
	printf("parent_test: %d\n", (np->parent == NULL));
	// printf("node2: %p\n", &node2);
	// printf("node3: %p\n", n3);

	// insert_child(np, ch);

	// simple_insert(n3, 9);

	// split(ch, test_e);

	// node_pointer noder = np->children[4];

	// node_pointer curr = np;

	// findkey(elem0, np);

	// printf("%d\n", curr->elements[0]);

	//insert_element(elem0, tree);
	//insert_element(elem1, tree);

	simple_insert(np, 30);
	simple_insert(np, 38);
	simple_insert(np, 42);
	simple_insert(np, 10);

	printf("done");


	int depth = get_btree_depth(tree);
	int width = svg_get_width(depth);
	int height = svg_get_height(depth);

	save_btree("b_tree.svg", tree);

	free(tree);

	return ret;
}


/*
*children counter einführen
*
*
*/