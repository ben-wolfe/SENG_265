#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RAND_VALUE 1000
#define MAX_NUM_VALUES 20
#define MAX_FILENAME_LEN 40

/*
 * Notice the use here of conditional compilation. To include
 * the lines for DEBUG, we use the -DDEBUG flag during compilation.
 * To include the lines for DEBUG2, we use the -DDEBUG2 flag during
 * compilation. To include both, we define both symbols.
 * 
 * Examples:
 *
 *   gcc readints.c -o readints -DDEBUG
 *   gcc readints.c -o readints -DDEBUG2
 *   gcc readints.c -o readints -DDEBUG -DDEBUG2
 */


int main(int argc, char *argv[]) {
    int i;  
    char filename[MAX_FILENAME_LEN];
    int numbers[MAX_NUM_VALUES];
    int temp, num;
    FILE *infile;

#ifdef DEBUG
    printf("Booyah!\n");
#endif

    /* Loop through command-line arguments... */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--file") == 0) {
            /* Assuming here there *is* an i+1'th argument. If
             * there isn't, there could be trouble...
             */
            strncpy(filename, argv[i+1], MAX_FILENAME_LEN);
        }
    }

    /* Open a file for input. Make sure, however, that the
     * file did successfully open...
     */
    infile = fopen(filename, "r");
    if (infile == NULL) {
        fprintf(stderr, "Could not open %s for writing.\n", filename);
        exit(1);
    }

    num = 0;
    while (fread(&temp, sizeof(int), 1, infile) > 0) {
        numbers[num] = temp;
        num++;
    }

#ifdef DEBUG2
    for (i = 0; i < num; i++) {
        printf("numbers[%d]: %d\n", i, numbers[i]);
    } 
#endif

    fclose(infile);
}
