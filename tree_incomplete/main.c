#include "btree/btree.h"

bool insert_element(stdelement element){
	bool success = !findkey(element);
	if(success) {
		if (!empty(btree)){
			insert_into_node(current_node, element);
		}
		else {
			//TODO: validate initialization of node
			struct node node1 = malloc(sizeof(struct node));
			node1.parent = NULL;
			node1.number_of_elements = 1;
			node1.elements[0] = element;
		}
	}
	return success;
}
// TODO: current_node als globale Variable?
bool findkey(stdelement element){
	bool success = false;
	struct node p = *btree->root; 
	struct node current_node = malloc(sizeof(struct node));
	while (!success && (p != NULL)){
		current_node = p
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
		p = *(current_node.children[MAXNODE])
	}
	return success;
}

int[] sort_array(){

}
 
void insert_into_node(node n, stdelement element){
	
	
	
	// int size_array = sizeof(n.elements) / sizeof(int)
	int position = 0;
	for (int i= 0; i < size_array; i++){
		if (element < n.elements[i]){
			position = i;
		}
	}

	
	int[5] splitarray;
	if(n.number_of_elements > 3){
		for(int i = 0; i < n.number_of_elements; i++){
			splitarray[i] = n.elements[i];
		}
		splitarray[4] = element;
 
	}
	int[4] new_elems;
	for(int i = 0; i < n.number_of_elements; i++){
		if (n.elements[i] > element){
			new_elems[i] = n.elements[i];
			continue;
		}
		else {
			new_elems[i] = element;
			if(n.elements[i - 1] != 0 && new_elements[i] == 0){
				break;
			}
			element = n.elements[i];
		}
	}
	n.elements = new_elems;
}

node create_node(node_pointer parent, short number_of_elements, stdelement elements, node_pointer children) {
	
	node new_node;
	new_node.parent = parent;
	new_node.number_of_elements = number_of_elements;
	new_node.elements = elements;
	new_node.children = children;

	return new_node;
}
// dereferenzieren des parents und seiner children und vergleich anhand des ersten Elements des Nodes
void 

int main(void){
  /*insert your code below*/
    btree tree = create_btree();

  return EXIT_SUCCESS;
}


