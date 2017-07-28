
Ben Wolfe | V00205547

LAST MODIFIED:
28.07.2017

EXTRA WORK:
* Implemented the assignment using both singly and doubly linked lists
* Added timing functionality for comparisons sake
* The doubly linked list is a true doubly linked list implementation with next/previous pointers 
* Ensured both tests have no memory leaks
* Wrote a shell script to run the two tests side-by-side
* Check the names of input/output files to make sure they have the correct file extension for the given direction (i.e. infile=.ph1/outfile=.ph2 for encode, infile=.ph2/outfile=.ph1 for decode)
* Modified the makefile to compile both programs

STEPS TO RUN:
* In EXTRA WORK folder (this folder), run make
* Open alltests.sh and modify the directory to where you'd like test files to be stored
* Run all alltests.sh
* Observe how slow the singly linked implementation is in comparison (The 'add_end' method has O(n) time complexity in the singly-linked implementation vs O(1) time complexity in the doubly-linked implementation.

