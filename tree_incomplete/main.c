#include <stdbool.h>
#include "btree/btree.h"
#include "test/test.h"

void debug_node(node_pointer node) {
	printf("\nnode: %p", node);
	if(node == NULL) {
		return;
	}
	printf("\n	#_elements: %d", node->number_of_elements);
	for(int i = 0; i < node->number_of_elements; i++) {
		printf("\n		%d: %d", i, node->elements[i]);
	}
	printf("\n	#_children: %d", node->number_of_children);
	for(int i = 0; i < node->number_of_children; i++) {
		printf("\n		%d: %p", i, node->children[i]);
	}
}

void debug_subtree(node_pointer node, int depth) {
	printf("\ndepth: %d", depth);
	debug_node(node);
	for(int i = 0; i < node->number_of_children; i++) {
		debug_subtree(node->children[i], depth+1);
	}
}

void debug_tree(btree tree) {
	debug_subtree(tree->root, 0);
}


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


int get_insert_position_of_element(node_pointer node, stdelement element) {
	for(int elem_idx = 0; elem_idx < node->number_of_elements; elem_idx++) {
		if(element < node->elements[elem_idx]){
			return elem_idx;
		}
	}
	return node->number_of_elements;
}


int get_middle_element_idx(node_pointer node, stdelement element) {
	int pos = get_insert_position_of_element(node, element);
	if(pos > ORDER) {
		return ORDER;
	}
	return ORDER - 1;
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

void insert_into_node(node_pointer, stdelement, btree*);


node_pointer create_node(node_pointer parent) {
	node_pointer node = malloc(sizeof(struct node));

	node->parent = parent;
	node->number_of_elements = 0;
	node->number_of_children = 0;
	for(int i = 0; i < MAXNODE; i++){
		node->elements[i] = 0;
		node->children[i] = NULL;
	}
	node->children[MAXNODE] = NULL;
	return node;
}

void split2(node_pointer, node_pointer, stdelement, btree*);


void simple_insert_child_node(node_pointer node, node_pointer child, stdelement element, btree *tree_pointer) {
	int insert_pos = get_insert_position_of_element(node, element);
	node->children[node->number_of_children] = node->children[node->number_of_children - 1];
	for(int i = node->number_of_elements - 1; i >= insert_pos; i--){
		node->elements[i + 1] = node->elements[i];
		node->children[i + 1] = node->children[i];
	}
	node->elements[insert_pos] = element;
	node->children[insert_pos] = child;
	node->number_of_elements++;
	node->number_of_children++;
}

void create_new_root(btree *tree_pointer, node_pointer child, stdelement element) {
	printf("\nnew root\n");
	node_pointer node = create_node(NULL);

	(*tree_pointer)->root = node;
	node->elements[0] = element;
	node->children[0] = child;
	node->number_of_elements = 1;
	node->number_of_children = child == NULL ? 0 : 1;
}

void insert_child_node(node_pointer node, node_pointer child, stdelement element, btree *tree_pointer) {
	printf("child node\n");
	if(!node) {
		node_pointer right_child = (*tree_pointer)->root;
		printf("\nold root: %p", right_child);
		debug_node(right_child);
		create_new_root(tree_pointer, child, element);
		node_pointer root = (*tree_pointer)->root;
		printf("\nnew root: %p", root);
		child->parent = root;
		right_child->parent = root;
		root->children[1] = right_child;
		root->number_of_children = 2;
		debug_node(root);
		printf("\nleft: %p", child);
		debug_node(child);
		return;
	}
	if (node->number_of_elements == MAXNODE) {
		split2(node, child, element, tree_pointer);
	} else {
		simple_insert_child_node(node, child, element, tree_pointer);
	}
}


void split2(node_pointer node, node_pointer new_child, stdelement element, btree *tree_pointer) {
	printf("split\n");

	printf("\nsplit node debug elem: %d", element);
	debug_node(node);
	printf("\nsplit new child debug");
	debug_node(new_child);

	int middle_idx = get_middle_element_idx(node, element);
	stdelement middle = node->elements[middle_idx];

	node_pointer left = create_node(node->parent);
	int elem_insert_pos = get_insert_position_of_element(node, element);

	if(elem_insert_pos <= middle_idx){
		printf("\n middle: %d", middle_idx);
		printf("\n insert_pos: %d", elem_insert_pos);
		for(int i = 0; i < elem_insert_pos; i++) {
			printf("\n for_a: %d", i);
			printf("\n value: %p", node->children[i]);
			left->elements[i] = node->elements[i];
			left->children[i] = node->children[i];
		}

		printf("\n for_b: %d", elem_insert_pos);
		printf("\n value: %p", new_child);
		left->elements[elem_insert_pos] = element;
		left->children[elem_insert_pos] = new_child;

		for(int i = elem_insert_pos; i < middle_idx; i++) {
			printf("\n for_c: %d", i + 1);
			printf("\n value: %p", node->children[i]);
			left->elements[i + 1] = node->elements[i];
			left->children[i + 1] = node->children[i];
		}

		printf("\n for_d: %d", middle_idx);
		printf("\n value: %p", node->children[middle_idx]);
		left->children[middle_idx + 1] = node->children[middle_idx];
		left->number_of_elements = middle_idx + 1;
		left->number_of_children = middle_idx + 2;

		int count_elements_right = MAXNODE - middle_idx - 1;

		for(int i = 0; i < count_elements_right; i++) {
			node->elements[i] = node->elements[middle_idx + i + 1];
			node->children[i] = node->children[middle_idx + i + 1];
		}
		node->children[count_elements_right] = node->children[MAXNODE];
		node->number_of_elements -= middle_idx + 1;
		node->number_of_children -= middle_idx + 1;
	} else {
		for(int i = 0; i < middle_idx; i++) {
			left->elements[i] = node->elements[i];
			left->children[i] = node->children[i];
		}
		left->children[middle_idx - 1] = node->children[middle_idx - 1];
		left->number_of_elements = middle_idx; 
		left->number_of_children = middle_idx + 1;

		int idx = 0;
		for(int i = middle_idx + 1; i < elem_insert_pos; i++, idx++) {
			node->elements[idx] = node->elements[i];
			node->children[idx] = node->children[i];
		}
		node->elements[idx] = element;
		node->children[idx] = new_child;
		idx++;

		for(int i = elem_insert_pos; i < node->number_of_elements; i++, idx++) {
			node->elements[idx] = node->elements[i];
			node->children[idx] = node->children[i];
		}
		node->children[idx] = node->children[node->number_of_children - 1];
		node->number_of_elements -= middle_idx;
		node->number_of_children -= middle_idx;
	}

	printf("\ndebug split2 middle: %d", middle);
	debug_node(node);
	printf("\nparent:");
	debug_node(node->parent);
	printf("\nleft:");
	debug_node(left);

	insert_child_node(node->parent, left, middle, tree_pointer);

}


void leaf_split(node_pointer node, stdelement element, btree *tree_pointer) {

	int middle_idx = get_middle_element_idx(node, element);
	stdelement middle = node->elements[middle_idx];

	node_pointer left = create_node(node->parent);
	int elem_insert_pos = get_insert_position_of_element(node, element);

	if(elem_insert_pos <= middle_idx){
		for(int i = 0; i < elem_insert_pos; i++) {
			left->elements[i] = node->elements[i];
		}
		left->elements[elem_insert_pos] = element;
		for(int i = elem_insert_pos; i < middle_idx; i++) {
			left->elements[i + 1] = node->elements[i];
		}

		left->number_of_elements = middle_idx + 1;
		int count_elements_right = MAXNODE - middle_idx - 1;

		for(int i = 0; i < count_elements_right; i++) {
			node->elements[i] = node->elements[middle_idx + i + 1];
			node->elements[middle_idx + i + 1] = NULL; // hier hat jeannine ganz frech rumgeschraubt
			node->number_of_elements--;
		}
		//node->number_of_elements = count_elements_right + 1; // JEANNINE war hier :PP

	} else {
		for(int i = 0; i < middle_idx; i++) {
			left->elements[i] = node->elements[i];	
		}
		left->number_of_elements = middle_idx; 
		int idx = 0;
		for(int i = middle_idx + 1; i < elem_insert_pos; i++, idx++) {
			node->elements[idx] = node->elements[i];
	
		}
		node->elements[idx] = element;

		idx++;

		for(int i = elem_insert_pos; i < node->number_of_elements; i++, idx++) {
			node->elements[idx] = node->elements[i];
		}
		node->number_of_elements -= middle_idx;
	}

	insert_child_node(node->parent, left, middle, tree_pointer);
}


bool findkey(stdelement element, node_pointer *current_node){
	printf("findkey\n");
	printf("elem: %d\n", element);
	printf("current_node: %p\n", *current_node);
	printf("elems-> %d\n", (*current_node)->elements[0]);
	node_pointer target = *current_node;
	int size_array = target->number_of_elements;
	printf("\n array size: %d\n", size_array);
	for(int i = 0; i < size_array; i++) {
		
		if(element == target->elements[i]) {
			printf("found_n: %p\n", *current_node);
			return true;
		}
		printf("iter: %d\n", i);
		if(element < target->elements[i]) {
			printf("elem in array: %d\n",target->elements[i] );
			// printf("element: %d\n", element);
			// printf("check %d\n", (element == target->elements[i]));
			if(i < target->number_of_children){
				*current_node = target->children[i];
				printf("found_node: %p\n", target->children[i]);
				return findkey(element, current_node);
			}
			return false;
		}
	}
	printf("\nleft for loop");
	if(target->number_of_children == MAXNODE + 1){
		*current_node = target->children[MAXNODE];
		return findkey(element, current_node);
	}
	if(target->number_of_children != 0){
		*current_node = target->children[size_array];
		return findkey(element, current_node);

	}

	return false;
}


bool empty(node_pointer node) {
	printf("empty\n");
	if(node->number_of_elements == 0) {
		return true;
	}
	return false;
}


void insert_into_node(node_pointer current, stdelement element, btree *tree_pointer) {
	printf("insert_into_node\n");
	printf("current_e: %d\n", current->number_of_elements);
	printf("i_node: %p\n", current);

	if(current->number_of_elements < MAXNODE) {
		printf("insert_into_node_simple_i\n");
		simple_insert(current, element);
	} else {
		leaf_split(current, element, tree_pointer);
	}
}


bool insert_element(stdelement element, btree *tree_pointer){
	printf("insert_element\n");
	node_pointer current_node = (*tree_pointer)->root;

	bool success = findkey(element, &current_node);
	printf("\nsuccess: %d", success);
	printf("\n found node: %p", current_node);
	if(!success) {
		if (!empty((*tree_pointer)->root)){
			insert_into_node(current_node, element, tree_pointer);
		}
		else {
			create_new_root(tree_pointer, NULL, element);
		}
	}
	return success;
}

int get_pos_in_parent(node_pointer node){
	if(node->parent == NULL){
		return -1;
	}
	int index;
	for(int i = 0; i < node->parent->number_of_children;i++){
		if(node == node->parent->children[i]){
			index = i;
		}
	}
	return index;
}

void node_pick_left(int position, node_pointer node){

	stdelement parent_elem = node->parent->elements[position - 1];
	node_pointer left = node->parent->children[position - 1];
	stdelement last_in_left = left->elements[left->number_of_elements - 1];
	node->elements[node->number_of_elements] = node->parent->elements[position - 1];
	left->elements[left->number_of_elements - 1] = NULL;
	left->number_of_elements--;
	node->parent->elements[position - 1] = last_in_left;
	node->elements[node->number_of_elements] = node->elements[node->number_of_elements -1];
	node->elements[node->number_of_elements -1] = parent_elem;
	node->number_of_elements++;
}

void node_pick_right(int position, node_pointer node){

	stdelement parent_elem = node->parent->elements[position];
	node_pointer right = node->parent->children[position +1];
	stdelement first_in_right = right->elements[0];
	node->elements[node->number_of_elements] = node->parent->elements[position];
	for(int i = 0; i < ((right->number_of_elements)-1) ;i++){
		right->elements[i] = right->elements[i+1];
		right->elements[i+1] = NULL;
	}
	right->number_of_elements--;
	node->parent->elements[position] = first_in_right;
	node->elements[node->number_of_elements] = parent_elem;
	node->number_of_elements++;
}

void node_last(int position, node_pointer node, btree tree_pointer){
	stdelement parent_elem = node->parent->elements[position-1];
	printf("\n last child");
	node->parent->elements[position -1] = node->elements[0];
	node->parent->children[position] = NULL;
	insert_element(parent_elem, &tree_pointer);
}

void merge_into_right(node_pointer node, int position, btree tree_pointer){
	printf("\n SPONGEBOB SCHWAMMKOPF \n");
	node_pointer pp = node->parent;
	node_pointer right = pp->children[position + 1];

	simple_insert(right, node->elements[0]);
	debug_node(right);
	simple_insert(right, pp->elements[position]);
	debug_node(right);

	debug_node(node);
	//pp elements löschen und aufrücken
	for(int i = position; i < pp->number_of_elements; i++){
		pp->elements[i] = pp->elements[i+1];
		pp->elements[i+1] = NULL;
	}
	pp->number_of_elements--;
	
	//node löschen und children aufrücken
	for(int i = position; i < pp->number_of_children; i++){
		pp->children[i] = pp->children[i+1];
		pp->children[i+1] = NULL;
	}
	pp->number_of_children--;

	if(pp->number_of_elements == 0){
		tree_pointer->root = right;
		right->parent = NULL;
	}

}

void merge_into_left(node_pointer node, int position, btree tree_pointer){
	node_pointer pp = node->parent;
	node_pointer left = pp->children[position - 1];

	simple_insert(left, node->elements[0]);
	simple_insert(left, pp->elements[position - 1]);

	debug_node(node);
	//pp elements löschen und aufrücken
	for(int i = position - 1; i < ((pp->number_of_elements) ); i++){
		pp->elements[i] = pp->elements[i+1];
		pp->elements[i+1] = NULL;
	}
	pp->number_of_elements--;

	//node löschen und children aufrücken
	for(int i = position; i < ((pp->number_of_children) ); i++){
		pp->children[i] = pp->children[i+1];
		pp->children[i+1] = NULL;
	}
	pp->number_of_children--;

	if(pp->number_of_elements == 0){
		tree_pointer->root = left;
		left->parent = NULL;
	}	
}


void node_too_small(node_pointer node, btree tree_pointer){
	printf("\n elements smaller\n");
	int position = get_pos_in_parent(node);

	node_pointer parent = node->parent;
	printf("\nparent: %p", parent);
	printf("\nposition: %d", position);
	// mittig oder rechts außen
	if(position > 0){
		if(parent->children[position - 1]->number_of_elements > ORDER){	
			printf("\nbigger 0");
			node_pick_left(position, node);
			return;
		}
		printf("\n MAXNODE \n");
		printf("\n shit");
	}

	// mittig oder links außen
	if (position < node->parent->number_of_children - 1){
		if(parent->children[position + 1]->number_of_elements > ORDER){
			node_pick_right(position, node);
			return;
		} 	

	}

	printf("\nposizion: %d", position);
	if (position > 0){
		printf("\n merge into left \n");
		merge_into_left(node, position, tree_pointer);
	}
	else {

		printf("\n ooooooooooooooooooooooooooooooooooh wer wohnt in ner annanas \n");
		printf("\n merge into right \n");
		printf("\n going inn \n");
		merge_into_right(node, position, tree_pointer);
	}

}

void delete_leaf(node_pointer node, stdelement element, btree tree_pointer){
	printf("\ndelete leaf\n");
	int pos;
	// lösche das Element aus dem Knoten bzw. setze es auf NULL und dekrementiere number_of_elements
	for(int i = 0; i < node->number_of_elements; i++){
		if(node->elements[i] == element){
		node->elements[i] = NULL;
		pos = i;
		//schiebe die restlichen Elemente eins nach links
		for(int k = i; k < ((node->number_of_elements) -1); k++){
			node->elements[k] = node->elements[k+1];
			node->elements[k+1] = NULL;
		}
		node->number_of_elements--; //muss nach dem aufschieben passieren, da sonst das letzte Element durch number_of_elements nicht angezeigt wird
		}
	}

	//Abfrage falls node kein Blattknoten ist
	if(node->number_of_children != 0){


		// ich ziehe das größte Elem aus meine Kind an position hoch
		stdelement child_elem = node->children[pos]->elements[node->children[pos]->number_of_elements - 1];
		simple_insert(node, child_elem);
		// muss Kindknoten nach dem hochziehen gemerged werden?
		delete_leaf(node->children[pos],child_elem, tree_pointer);
	}

	//Elemente in einem Knoten dürfen nie < 2 sein
	if(node->number_of_elements < 2){
		node_too_small(node, tree_pointer);
	}

}


void delete(node_pointer node, stdelement element, btree *tree_pointer){
	printf("\ndelete\n");
	debug_node(node);
	if (findkey(element, &node) == true){ //falls das zu löschende Element und der dazugehörige Knoten gefunden wurde

		if(node->number_of_children == 0){ //node wurde in findkey auf den richtigen Knoten gesetzt, Abfrage node = blatt?
			printf("\nEntferne Element aus Blattknoten\n");
			debug_node(node);
			delete_leaf(node, element, *tree_pointer);
		}
		else {
			printf("\nEntferne Element aus Elternknoten\n");

			delete_leaf(node, element, *tree_pointer);

			

		}
	}
}


int main(void){
	int ret;

	struct node node1;
	node1.parent = NULL;
	node1.number_of_elements = 0;
	node1.number_of_children = 0;
	node1.elements[0] = NULL;
	node1.elements[1] = NULL;
	node1.elements[2] = NULL;
	node1.elements[3] = NULL;
	node1.children[0] = NULL; 
	node1.children[1] = NULL;
	node1.children[2] = NULL; 
	node1.children[3] = NULL;
	node1.children[4] = NULL; 

	btree tree = create_btree();
	tree->root = &node1;


	insert_element(30, &tree);
	insert_element(38, &tree);
	insert_element(42,&tree);
	insert_element(10,&tree);
	insert_element(20,&tree);
	insert_element(25,&tree);
	insert_element(32,&tree);
	insert_element(34,&tree);
	insert_element(40,&tree);
	insert_element(41,&tree);	
	insert_element(44,&tree);
	insert_element(50,&tree);
	insert_element(56,&tree);

	delete(tree->root, 30, &tree);
	delete(tree->root, 38, &tree);
	delete(tree->root, 42,&tree);
	delete(tree->root, 10,&tree);
	delete(tree->root, 20,&tree);
	delete(tree->root, 25,&tree);
	delete(tree->root, 32,&tree);
	delete(tree->root, 34,&tree);
	delete(tree->root, 40,&tree);
	delete(tree->root, 41,&tree);	
	delete(tree->root, 44,&tree);
	delete(tree->root, 50,&tree);
	//delete(tree->root, 56,&tree);	

	printf("\n debugging tree: \n");
	debug_tree(tree);

	printf("\ndone\n");

	//delete(tree->root, 57,&tree);


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