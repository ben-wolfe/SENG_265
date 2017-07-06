#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RAND_VALUE 1000
#define MAX_NUM_VALUES 20
#define RANDOM_SEED 3141
#define MAX_FILENAME_LEN 40


int main(int argc, char *argv[]) {
    int i;  
    int num;
    char filename[MAX_FILENAME_LEN];
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
     * generate some random numbers, and write them one at a time
     * to the file.
     *
     * In random2.c, this generate-numbers-and-write-them-out is
     * done slightly differently (and perhaps more efficiently from
     * the point of the filesystem).
     */
    srand(RANDOM_SEED);
    for (i = 0; i < MAX_NUM_VALUES; i++) {
        num = rand() % MAX_RAND_VALUE;
        fwrite(&num, sizeof(int), 1, outfile);
    }

    /* You must, must, MUST always close the file if you want to
     * guarantee all contents are written properly to the filesystem.
     */
    fclose(outfile);
}
