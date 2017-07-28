/*
	Ben Wolfe | V00205547

	DESC:
	This program defines the methods to implement alongside a singly linked list.

	LAST MODIFIED:
	28.07.2017
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "sll.h"

/* -------------------------------------------------------------------- CREATE */
void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}

charval_t *new_charval(unsigned char c, int flag) {
    charval_t *temp;

    temp = (charval_t *) emalloc(sizeof(charval_t));
    temp->c = c;
    temp->flag = flag;
    temp->next = NULL;

    return temp;
}

/* ----------------------------------------------------------------------- ADD */
charval_t *add_front(charval_t *list, charval_t *cv) {
    cv->next = list;
    return cv;
}

charval_t *add_end(charval_t *list, charval_t *cv) {
    charval_t *curr;

    if (list == NULL) {
        cv->next = NULL;
        return cv;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = cv;
    cv->next = NULL;
    return list;
}

/* ----------------------------------------------------------------------- GET */
charval_t *peek_front(charval_t *list) {
    return list;
}

charval_t *peek_at(charval_t *head, int key_index){

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

/* -------------------------------------------------------------------- DELETE */
charval_t *remove_front(charval_t *list) {
     if (list == NULL) {
        return NULL;
    }

	charval_t *deleted = list;
	list = list->next;
	free_charval(deleted);

    return list;
}

charval_t *remove_at(charval_t *head, int key_index){
	
	if(head == NULL){
		return NULL;
	}

	charval_t *temp = head;	
	
	if(key_index == 0){
		head = head->next;
		free_charval(temp);
		return head;
	}

	int i;
	for(i = 0; temp != NULL && i < key_index-1; i++, temp=temp->next);
	/* i is at key_index-1 here so temp is at index -1 and temp->next is at index */
	if(temp == NULL || temp->next == NULL){
		printf("\nERROR\nTrying to access an index outside of list range\n\n");
		exit(1);
	}
	
	charval_t *deleted = temp->next;
	charval_t *next = temp->next->next;
	temp->next = next;
	free_charval(deleted);

	return head;
}

charval_t *delete_list(charval_t *head){
	if(head == NULL){
		return NULL;
	}

	charval_t *deleted = head;

	while(head != NULL){
		deleted = head;
		head = head->next;

		free_charval(deleted);
	}

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
