#include <stdbool.h>
#include "btree/btree.h"
#include "test/test.h"

void simple_insert(node_pointer node, stdelement element) {
	int size = node->number_of_elements;
	int buffer_element;
	if(size < 2 * ORDER){
		for(int i = 0; i < size; i++) {
			if(node->elements[i] == (int) NULL) {
				node->elements[i] = element;
				node->number_of_elements++;
				return;
			}
			if(element < node->elements[i]) {
				buffer_element = node->elements[i];
				node->elements[i] = element;
				element = buffer_element;
			}
		}
		node->elements[size] = element;
		node->number_of_elements++;
	}
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


void insert_child(node_pointer parent, node_pointer child) {
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


void split(node_pointer node, stdelement middle) {
	node_pointer rig = malloc(sizeof(struct node));

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

	simple_insert(node->parent, middle);
	insert_child(node->parent, rig);
}


bool findkey2(stdelement element, node_pointer current_node){
	bool success = false;
	node_pointer p = current_node;
	//struct node current_node = malloc(sizeof(struct node));
	while (!success && (p != NULL)){
		// current_node = p;
		int size_array = (sizeof(current_node->elements)) / (sizeof(int));
		printf("%i\n", size_array);
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


bool findkey3(stdelement element, node_pointer current_node){
	bool success = false;
	node_pointer target = current_node;
	int size_array = target->number_of_elements;
	printf("size is: %d\n", size_array);
	while(!success && (target != NULL)){
		for(int i = 0; i < size_array; i++) {
			if(target->elements[i] == element) {
				current_node = target;
				return !success;
			}
			if(element < target->elements[i]) {
				success = findkey3(element, target->children[i]);
				printf("child no.: %d\n", i);
				return success;
			}
		}
		success = findkey3(element, target->children[size_array]);
	}
	return success;
}

bool findkey(stdelement element, node_pointer current_node){
	bool success = false;
	node_pointer target = current_node;
	int size_array = target->number_of_elements;
	// printf("array size: %d\n", size_array);
		while(!success && (target != NULL)){
			for(int i = 0; i < size_array; i++) {
				if(element <= target->elements[i]) {
					// printf("node: %s\n", target);
					// printf("elem in array: %d\n",target->elements[i] );
					// printf("element: %d\n", element);
					if(element == target->elements[i]){
						current_node = target;
						printf("found: %d\n", current_node->elements[0]);
						success = true;
						break;
					}
					if(!success) {
						current_node = target->children[i];
						printf("node: %d\n", current_node->elements[0]);
						success = findkey(element, current_node);
						break;

					}
				}
			}
		/* TODO:
		 *	gibt den falschen current node an
		 * richtiger currentnode wird nicht abgespeichert
		*/
		// current_node = target->children[size_array];
		// return success = findkey(element, current_node);
	}
	// printf("current nodes elem %d\n", current_node->elements[0]);
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
	if(node->number_of_elements == 0) {
		return true;
	}
	return false;
}


void insert_into_node(node_pointer current, stdelement element) {
	if(current->number_of_elements < MAXNODE) {
		simple_insert(current, element);
		return;
	}
	split(current, (middle_element(current, element)));
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

	struct node node2;
	node2.parent = NULL;
	node2.number_of_elements = 2;
	node2.elements[0] = 1;
	node2.elements[1] = 2;
	node2.elements[2] = NULL;
	node2.elements[3] = NULL;
	node2.children[0] = NULL;
	node2.children[1] = NULL;
	node2.children[2] = NULL;
	node2.children[3] = NULL;
	node2.children[4] = NULL;

	struct node node3;
	node3.parent = NULL;
	node3.number_of_elements = 2;
	node3.elements[0] = 8;
	node3.elements[1] = 9;
	node3.elements[2] = NULL;
	node3.elements[3] = NULL;
	node3.children[0] = NULL;
	node3.children[1] = NULL;
	node3.children[2] = NULL;
	node3.children[3] = NULL;
	node3.children[4] = NULL;

	struct node node4;
	node4.parent = NULL;
	node4.number_of_elements = 4;
	node4.elements[0] = 20;
	node4.elements[1] = 30;
	node4.elements[2] = 40;
	node4.elements[3] = 50;
	node4.children[0] = NULL;
	node4.children[1] = NULL;
	node4.children[2] = NULL;
	node4.children[3] = NULL;
	node4.children[4] = NULL;

	struct node node1;
	node1.parent = NULL;
	node1.number_of_elements = 3;
	node1.number_of_children = 2;
	node1.elements[0] = 5;
	node1.elements[1] = 7;
	node1.elements[2] = 10;
	node1.elements[3] = NULL;
	node1.children[0] = &node2;
	node1.children[1] = NULL;
	node1.children[2] = &node3;
	node1.children[3] = NULL;
	node1.children[4] = NULL;

	node2.parent = &node1;
	node3.parent = &node1;

	btree tree = create_btree();
	tree->root = &node1;

	stdelement test_e = 35;
	stdelement elem0 = 1;

	node_pointer np = &node1;
	node_pointer ch = &node4;

	insert_child(np, ch);

	split(ch, test_e);

	node_pointer noder = np->children[4];

	node_pointer curr = np;

	node_pointer x = NULL;

	// printf("%s", x);

	findkey(elem0, np);

	printf("%d\n", curr->elements[0]);

	// insert_element(elem0, tree);


	int depth = get_btree_depth(tree);
	int width = svg_get_width(depth);
	int height = svg_get_height(depth);

	save_btree("render_node_to_svg_test1.tree.svg", tree);

	free(tree);

	return ret;
}


/*
*children counter einführen
*
*
*/