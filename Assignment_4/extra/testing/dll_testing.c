#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "linkedlist.h"

void print_charval(charval_t *cv, void *arg){
	printf((char *)arg, cv->c, cv->flag);
}	

int main(int argc, char *argv[]){

	int i;
	charval_t *temp = NULL;
	charval_t *list = NULL;
	charval_t *list_tail;
	
/*	
	// Testing ADD_FRONT
	for(i = 0; i < 20; i++){
		temp = new_charval('-', i);
		list = add_front(list, &list_tail, temp);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");
	}

	// Testing PEEK_FRONT and REMOVE_FRONT 
	while(list != NULL){
		
		charval_t *temp_front = peek_front(list);
		printf("HEAD: '%c', %d\n", temp_front->c, temp_front->flag);
		list = remove_front(list, &list_tail);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");

	}

	*/
	
	// Testing ADD_END
	for(i = 0; i < 20; i++){
		temp = new_charval('-', i);
		list = add_end(list, &list_tail, temp);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");
	}

	temp = peek_at(list, &list_tail, 5);
	charval_t *move_char = new_charval(temp->c, temp->flag);
	printf("INDEX %d: '%c', %d\n", 5, move_char->c, move_char->flag);

	list = remove_at(list, &list_tail, 5);
	list = add_front(list, &list_tail, move_char);
	apply(list, print_charval, "('%c' %d) ");
	printf("\n");

	temp = peek_at(list, &list_tail, 0);
	move_char = new_charval(temp->c, temp->flag);
	printf("INDEX %d: '%c', %d\n", 5, move_char->c, move_char->flag);

	list = remove_at(list, &list_tail, 0);
	//list = add_front(list, &list_tail, move_char);
	apply(list, print_charval, "('%c' %d) ");
	printf("\n");

/*
	// Testing PEEK_AT and REMOVE_AT 
	for(i = 19; i >= 0; i--){
		temp = peek_at(list, &list_tail, i);
		printf("INDEX %d: '%c', %d\n", i, temp->c, temp->flag);
		list = remove_at(list, &list_tail, i);
		apply(list, print_charval, "('%c' %d) ");
		printf("\n");
	}
*/
	delete_list(list, &list_tail);

	return(0);	
}
