/*
	Ben Wolfe | V00205547
	
	DESC:
	This program defines the methods to use alongside a doubly-linked list.

	DATE:
	25.07.2017
*/



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

/* ---------------------------------------------------------------------------------- CREATE */
void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}

charval_t *new_charval(unsigned char c, int int_flag) {
    charval_t *temp;

    temp = (charval_t *) emalloc(sizeof(charval_t));
    temp->c = c;
    temp->flag = int_flag;
    temp->next = NULL;
	 temp->prev = NULL;

    return temp;
}

/* ------------------------------------------------------------------------------------- ADD */

charval_t *add_front(charval_t *head, charval_t **tail, charval_t *cv) {
    
	if(head == NULL){
		*tail = cv;
	} else {
		head->prev = cv;
	}

	cv->next = head;
	return cv;
}

charval_t *add_end(charval_t *head, charval_t **tail, charval_t *cv) {

	if(head == NULL){
		*tail = cv;
		head = cv;
	} else {
		(*tail)->next = cv;
		cv->prev = *tail;
	}
	*tail = cv;
	return head;
}

/* -------------------------------------------------------------------------------------- GET */

charval_t *peek_at(charval_t *head, charval_t **tail, int key_index){

	if(head == NULL){
		return NULL;
	}

	int index;
	charval_t *key_link = NULL;

	for(index = 0; head != NULL; head = head->next, index++){
		if(index == key_index){
			key_link = head;
			return key_link;
		}
	}

	if(key_link == NULL){
		printf("\nERROR\nTrying to access an index outside of the list range\n\n");
		exit(1);
	}

	return NULL;
}

charval_t *peek_front(charval_t *head) {
    return head;
}


/* ---------------------------------------------------------------------------------- DELETE */

charval_t *remove_front(charval_t *head, charval_t **tail) {

	if(head == NULL){
		return NULL;
	}

	charval_t *deleted = head;

	if(head->next == NULL){
		*tail = NULL;
	} else {	
		head->next->prev = NULL;
	}

	head = head->next;
	free_charval(deleted);

	return head;
}

charval_t *delete_list(charval_t *head, charval_t **tail){

	if(head == NULL){
		return NULL;
	}

	charval_t *deleted;

	while(head != NULL){
		deleted = head;

		if(head->next == NULL){
			*tail=NULL;
		} else {
			head->next->prev = NULL;
		}

		head = head->next;
		free_charval(deleted);
	}

	return head;

}

charval_t *remove_at(charval_t *head, charval_t **tail, int key_index){
	
	if(head == NULL){
		return NULL;
	}

	int index;
	charval_t *key_link = NULL;
	charval_t *crawler = head;
	charval_t *deleted = NULL;

	for(index = 0; crawler != NULL; crawler = crawler->next, index++){
		if(index == key_index){
			key_link = crawler;	
			break;
		}
	}
	
	if(key_link == NULL){
		printf("\nERROR\nTrying to remove an index outside of the list range\n\n");
		exit(1);
	}

	deleted = key_link;

	if(key_link == head){
		head = head->next;
	} else {
		key_link->prev->next = key_link->next;
	}

	if(key_link == *tail){
		*tail = key_link->prev;
	} else {
		key_link->next->prev = key_link->prev;
	}

	deleted->next = NULL;
	deleted->prev = NULL;
	free_charval(deleted);

	return head;
}

void free_charval(charval_t *cv) {
    assert(cv != NULL);
    free(cv);
}

void apply(charval_t *list, void (*fn)(charval_t *list, void *), void *arg){
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}
