#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RAND_VALUE 1000
#define MAX_NUM_VALUES 20
#define RANDOM_SEED 3141
#define MAX_FILENAME_LEN 40


int main(int argc, char *argv[]) {
    int i;  
    char filename[MAX_FILENAME_LEN];
    int numbers[MAX_NUM_VALUES];
    FILE *outfile;

    /* Loop through command-line arguments... */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--file") == 0) {
            /* Assuming here there *is* an i+1'th argument. If
             * there isn't, there could be trouble...
             */
            strncpy(filename, argv[i+i], MAX_FILENAME_LEN);
        }
    }

    /* Open a file for output. Make sure, however, that the
     * file did successfully open...
     */
    outfile = fopen(filename, "w");
    if (outfile == NULL) {
        fprintf(stderr, "Could not open %s for writing.\n", filename);
        exit(1);
    }

    /* Initialize the random-number generator with the given seed.
     * Generate the numbers, storing them into an integer array.
     *
     * Once values have been written to the array, write this array
     * to the file.
     */
    srand(RANDOM_SEED);
    for (i = 0; i < MAX_NUM_VALUES; i++) {
        numbers[i] = rand() % MAX_RAND_VALUE;
    }
    fwrite(numbers, sizeof(int), MAX_NUM_VALUES, outfile);

    /* You must, must, MUST always close the file if you want to
     * guarantee all contents are written properly to the filesystem.
     */
    fclose(outfile);
}
