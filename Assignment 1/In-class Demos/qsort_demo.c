#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 10
#define MAX_COLUMNS 20
#define MAX_NUMBER 10000
#define RANDOM_SEED 20170530

char *LETTERS = "abcdefghiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; 

int compare_ints(const void *a, const void *b) {
    int ia = *(int *)a;
    int ib = *(int *)b;

    return ia - ib;
}

int compare_strings(const void *a, const void *b) {
    char *sa = (char *)a;
    char *sb = (char *)b;

    return strcmp(sa, sb);
}


void gen_and_sort_ints() {
    int number[MAX_ROWS];
    int i, num;

    num = rand() % MAX_ROWS + 1;

    for (i = 0; i < num; i++) {
        number[i] = rand() % MAX_NUMBER;
        printf("BEFORE %d: %d\n", i, number[i]);
    }

    qsort(number, num, sizeof(int), compare_ints);

    for (i = 0; i < num; i++) {
        printf("AFTER %d: %d\n", i, number[i]);
    }
    
    
}


void gen_and_sort_words() {
    char word[MAX_ROWS][MAX_COLUMNS];

    int i, j, num, word_len;
    
    num = rand() % MAX_ROWS + 1;

    for (i = 0; i < num; i++) {
        word_len = rand() % (MAX_COLUMNS - 1) + 1;
        for (j = 0; j < word_len; j++) {
            word[i][j] = LETTERS[rand() % strlen(LETTERS)];
        }
        word[i][j] = '\0';
        printf("BEFORE %d: %s\n", i, word[i]);
    }

    qsort(word, num, sizeof(char) * MAX_COLUMNS, compare_strings);

    for (i = 0; i < num; i++) {
        printf("AFTER %d: %s\n", i, word[i]);
    }
}


int main(int argc, char *argv[]) {
    srand(RANDOM_SEED);

    gen_and_sort_ints();
    gen_and_sort_words();


