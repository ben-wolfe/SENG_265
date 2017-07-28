#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "singlylinkedlist.h"

void print_charval(charval_t *cv, void *arg){
	printf((char *)arg, cv->c, cv->val);
}	

int main(int argc, char *argv[]){

	int i;
	charval_t *temp = NULL;
	charval_t *list = NULL;
	
	// Testing ADD_FRONT
	for(i = 0; i < 20; i++){
		temp = new_charval('-', i);
		list = add_front(list, temp);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");
	}

	//delete_list(list);

	// Testing PEEK_FRONT and REMOVE_FRONT
	while(list != NULL){
		
		charval_t *temp_front = peek_front(list);
		printf("HEAD: '%c', %d\n", temp_front->c, temp_front->val);
		list = remove_front(list);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");

	}

	// Testing ADD_END
	for(i = 0; i < 20; i++){
		temp = new_charval('-', i);
		list = add_end(list, temp);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");
	}

	temp = peek_at(list, 5);
	charval_t *move_char = new_charval(temp->c, temp->val);
	printf("INDEX %d: '%c', %d\n", 5, move_char->c, move_char->val);

	list = remove_at(list, 5);
	apply(list, print_charval, "('%c' %d)");
	printf("\n");

	list = add_front(list, move_char);
	apply(list, print_charval, "('%c' %d)");
	printf("\n");
	
	// Testing PEEK_AT and REMOVE_AT
	for(i = 19; i >= 0; i--){
		temp = peek_at(list, i);
		printf("INDEX %d: '%c', %d\n", i, temp->c, temp->val);
		list = remove_at(list, i);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");
	} 

	return(0);	
}
