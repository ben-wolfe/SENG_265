/* 
	Ben Wolfe | V00205547

	DESC:
	Header file to a singly linked list.

	DATE:
	26.07.2017
*/


#ifndef _SLL_H_
#define _SLL_H_

typedef struct charval charval_t;
struct charval {
    unsigned char c;
    int           flag;
    charval_t *next;
};

charval_t *new_charval(unsigned char c, int flag);

charval_t *add_front(charval_t *, charval_t *);
charval_t *add_end(charval_t *, charval_t *);

charval_t *peek_front(charval_t *);
charval_t *peek_at(charval_t *, int flag);

charval_t *remove_front(charval_t *);
charval_t *remove_at(charval_t *, int flag);
charval_t *delete_list(charval_t *);

void free_charval(charval_t *);
void apply(charval_t *, void(*fn)(charval_t *, void *), void *arg);

#endif
