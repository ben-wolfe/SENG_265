## Work Beyond Assignment Description
* Implemented the assignment using both singly and doubly linked lists
* Added timing functionality for comparisons sake
* The doubly linked list is a true doubly linked list implementation with next/previous pointers 
* Tested using Valgrind to ensure no memory leaks
* Wrote a shell script to run the two tests side-by-side
* Check the names of input/output files to make sure they have the correct file extension for the given direction (i.e. infile=.ph1/outfile=.ph2 for encode, infile=.ph2/outfile=.ph1 for decode)
* Modified the makefile to compile both programs

## Running the Program
* In EXTRA WORK folder (this folder), run
```shell
make
```
* Run 
```shell
all alltests.sh
```
* Observe how slow the singly linked implementation is in comparison (The 'add_end' method has O(n) time complexity in the singly-linked implementation vs O(1) time complexity in the doubly-linked implementation.
