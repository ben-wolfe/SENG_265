/* 
	Ben Wolfe | V00205547

	DESC:
	This program reads performs text compression and extraction from .ph1 files to
	.ph2 files and vice versa.  The program makes use of dynamic memory allocation
	and doubly linked lists to perform the transformation.

	DATE:
	26.07.2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "dll.h"
#include <time.h>

/***********************************************************************************
                        GLOBAL VARIABLES & DEFINITIONS
 **********************************************************************************/
#define ENCODE 0
#define DECODE 1
#define INFO_BYTES 8
#define IS_DIGIT 1
#define NOT_DIGIT 0

FILE *input_file, *output_file;
char *input_name, *output_name;
int mode = -1; /* -1 is used to indicate unassigned direction */

char *ph1_magic = "\xab\xba\xbe\xef";
char *ph2_magic = "\xda\xaa\xaa\xad";

/***********************************************************************************
                              FUNCTION PROTOTYPES 
 **********************************************************************************/
void show_usage();
int process_command_line(int argc, char **argv);

int process_info_bytes();
unsigned char *read_input_file();
void write_info_bytes(int blocksize);

int in_list(charval_t *list, unsigned char character);
void print_charval(charval_t *cv, void *arg);
void print_coding(charval_t *list);

void encode(unsigned char *buffer);
void encode_mtf(unsigned char *, charval_t **, charval_t **, \
											charval_t **, charval_t **);
void encode_rl(charval_t *, charval_t **, charval_t **);
void encode_ascii(charval_t *, charval_t **, charval_t **);

void decode(unsigned char *buffer);
void decode_ascii(unsigned char *, charval_t **, charval_t **);
void decode_rl(charval_t *, charval_t **, charval_t **);
void decode_mtf(charval_t *, charval_t **, charval_t **);

/***********************************************************************************
	                                  MAIN
 **********************************************************************************/
int main(int argc, char *argv[]) {

	clock_t start, end;
	double CPU_time;
	start = clock();

	int blocksize;
	unsigned char *buffer = NULL;
	int mode = process_command_line(argc, argv);

	input_file = fopen(input_name, "r");
	if(input_file == NULL){
		fprintf(stderr, "\nFILE ERROR\nUnable to open file: %s\n\n", input_name);
		exit(1);
	}

	blocksize = process_info_bytes(); 
	buffer = read_input_file(); 
	fclose(input_file); 
	
	output_file = fopen(output_name, "w");
	write_info_bytes(blocksize);

	if(mode == ENCODE){
		encode(buffer);
	}
	else if(mode == DECODE){ 
		decode(buffer);
	}
	
	if(buffer){
		free(buffer); 
	} 

	fclose(output_file);

	end = clock();
	CPU_time = (double)(end-start) / CLOCKS_PER_SEC;

	printf("DOUBLY LINKED run time: %.3f sec\n", CPU_time);

	return 0;
}

/*********************************************************************************** 
                                  FUNCTIONS
 **********************************************************************************/

/* ----------------------------------------------------- READ AND CHECK ARGUMENTS */
int process_command_line(int argc, char **argv){
/* Reads arguments from  the command line and performs 
	several checks to make sure the input is valid. */
	
	if(argc != 6) show_usage();
	
	int arg;
	for(arg = 0; arg < argc; arg++){
		if(strcmp(argv[arg], "--encode") == 0) mode = ENCODE;
		if(strcmp(argv[arg], "--decode") == 0) mode = DECODE;

		if(strcmp(argv[arg], "--infile") == 0){
			if(arg != arg-1) input_name = argv[arg+1];
			else show_usage();
		}

		if(strcmp(argv[arg], "--outfile") == 0){
			if(arg != argc-1) output_name = argv[arg+1];
			else show_usage();
		}
	}
	
	/* This is a check to make sure all required fields are filled */
	if(mode == -1 || input_name == NULL || output_name == NULL){
		show_usage();	
		exit(1);
	}

	/* Check for valid file extensions using strcmp on the last 4 characters */
	int in_length = strlen(input_name); 
	int out_length = strlen(output_name);
	char *input_ext = &input_name[in_length-4];
	char *output_ext = &output_name[out_length-4];

	if(mode == ENCODE){
		if(strcmp(input_ext, ".ph1") != 0){
			printf("\nFILE ERROR\n"\
					 "Input file must have a .ph1 extension when encoding\n\n");
			exit(1);
		}
		if(strcmp(output_ext, ".ph2") != 0){
			printf("\nFILE ERROR\n"\
					 "Output file must have a .ph2 extension when encoding\n\n");
			exit(1);
		}
	} else if (mode == DECODE){
		if(strcmp(input_ext, ".ph2") != 0){
			printf("\nFILE ERROR\n"\
					 "Input file must have a .ph2 extension when decoding\n\n");
			exit(1);
		}
		if(strcmp(output_ext, ".ph1") != 0){
			printf("\nFILE ERROR\n"\
					 "Output file must have a .ph1 extension when decoding\n\n");
			exit(1);
		}
	}

	return mode;
}

void show_usage(){
	fprintf(stderr, "\nINPUT ERROR\nCorrect usage: phase2 [--encode | --decode] " \
						 "--infile <filename> --outfile <filename>\n\n");
	exit(1);
}

/* -------------------------------------------------------------- FILE PROCESSING */
int process_info_bytes(){ 
/* Checks the first 8 bytes of the file to make sure the type
	is valid and to obtain the transformation blocksize. */

	int blocksize;
	char file_info[INFO_BYTES];
	fread(file_info, sizeof(char), INFO_BYTES, input_file);

	if(mode == ENCODE){
	
		if(file_info[0] != '\xab' || file_info[1] != '\xba' || \
			file_info[2] != '\xbe' || file_info[3] != '\xef') {
		
			printf("\nFILE ERROR\nInput file is not a .ph1 file\n\n");
			exit(1);
		}		
	
	} else if(mode == DECODE) {
	
		if(file_info[0] != '\xda' || file_info[1] != '\xaa' || \
			file_info[2] != '\xaa' || file_info[3] != '\xad') {
		
			printf("\nFILE ERROR\nInput file is not a .ph2 file\n\n");
			exit(1);
		}

	}

	/* Based on the forum: https://goo.gl/WXW47n */
	blocksize = (file_info[4] & 0xFF) | (file_info[5] & 0xFF) << 8 | \
					(file_info[6] & 0xFF) << 16 | (file_info[7] & 0xFF) << 24;

	return blocksize;
}

unsigned char *read_input_file(){
/* Reads the contents of the file as one long string.
	Based on the forum: https://goo.gl/EJBDA3 */
	
	unsigned char *buffer = NULL;

	fseek(input_file, 0, SEEK_END); /* Point to the end of the file */
	long fsize = ftell(input_file); /* Find how many bytes the file is */
	rewind(input_file); /* Reset file pointer to file begininning */

	fsize = fsize - INFO_BYTES;
	buffer = malloc(fsize+1); /* Add +1 for EOS char */
	fseek(input_file, INFO_BYTES, SEEK_SET);
	fread(buffer, fsize, 1, input_file);

	return buffer;	
}

void write_info_bytes(int blocksize){
/* Writes the transformation blocksize to the file
	in 4 bytes via bit shifting. */
	
	if(mode == ENCODE){
		fputs(ph2_magic, output_file);		
	} else if(mode == DECODE){
		fputs(ph1_magic, output_file);
	}
	
	char block_string[4];
	block_string[0] = blocksize & 0xFF;
	block_string[1] = (blocksize >> 8) & 0xFF;
	block_string[2] = (blocksize >> 16) & 0xFF;
	block_string[3] = (blocksize >> 24) & 0xFF;

	fputc(block_string[0], output_file);
	fputc(block_string[1], output_file);
	fputc(block_string[2], output_file);
	fputc(block_string[3], output_file);

}

void write_to_file(charval_t *buffer){
/* Writes contents from the encoded/decoded list to the file
	one character at a time. */

	for( ; buffer != NULL; buffer = buffer->next){
		fputc(buffer->c, output_file);
	}
}

/* --------------------------------------------------------------- LIST FUNCTIONS */
void print_list(charval_t *list){
/* Prints the contents of each list node including both
	its character value and its integer flag */

	for( ; list != NULL; list = list->next){
		printf("(C: %x F: %d)\n", list->c, list->flag);
	}
	printf("\n");
}


void print_coding(charval_t *list){
/* Prints the character value for each node 
	in the list */

	for( ; list != NULL; list = list->next){
		printf("%x ", list->c);
	}
	printf("\n");
}

int in_list(charval_t *list, unsigned char character){
/* Checks if the character is currently in the list.
	Returns 1 if true and 0 if false. */

	for( ; list != NULL; list = list->next){
		if(list->c == character) return 1;
	}
	return 0;
}

int get_index(charval_t *list, unsigned char character){
/* Returns the position of a specific character in the list
	and 0 if the character is not in the list */

	int index;
	for(index = 0; list != NULL; list = list->next, index++){
		if(list->c == character) return index;
	}
	return 0;
}

/* ----------------------------------------------------------------------- ENCODE */
void encode(unsigned char *buffer){
/* Encodes a string of text by performing move-to-front
	encoding followed by run-length encoding.  A modified
	ascii scheme is used to represent numbers */

	/* Lists used for the transofmration */
	charval_t *lookup_list, *lookup_tail; 
	charval_t *mtf_encoding, *mtf_tail;
	charval_t *rl_encoding, *rl_tail;
	charval_t *ascii_encoding, *ascii_tail;

	/* Set all pointers to null initially */
	lookup_list = lookup_tail = NULL;
	mtf_encoding = mtf_tail = NULL;
	rl_encoding = rl_tail = NULL;
	ascii_encoding = ascii_tail = NULL;

	encode_mtf(buffer, &lookup_list, &lookup_tail, &mtf_encoding, &mtf_tail);
	encode_rl(mtf_encoding, &rl_encoding, &rl_tail);
	encode_ascii(rl_encoding, &ascii_encoding, &ascii_tail);
	write_to_file(ascii_encoding);

	/* Free the memory allocated by each list */
	delete_list(lookup_list, &lookup_tail);
	delete_list(mtf_encoding, &mtf_tail);
	delete_list(rl_encoding, &rl_tail);
	delete_list(ascii_encoding, &ascii_tail);

}

void encode_mtf(unsigned char *buffer, \
					charval_t **lookup_list, charval_t **lookup_tail, \
					charval_t **mtf_encoding, charval_t **mtf_tail){
/* Performs move-to-front encoding on stream of input text */

	charval_t *temp_char;
	charval_t *switch_char;

	int lookup_length = 0;
	int index = 0;
	
	for( ; *buffer; buffer++){

		/* When the character is not already in the lookup list */
		if(!in_list(*lookup_list, *buffer)){

			/* Add a new character to the lookup list */
			temp_char = new_charval(*buffer, NOT_DIGIT);
			*lookup_list = add_end(*lookup_list, &(*lookup_tail), temp_char);
		
			/* Add two characters to the mtf list in the form 'num', 'char' */
			temp_char = new_charval(lookup_length+1, IS_DIGIT);
			*mtf_encoding = add_end(*mtf_encoding, &(*mtf_tail), temp_char);
			
			temp_char = new_charval(*buffer, NOT_DIGIT);
			*mtf_encoding = add_end(*mtf_encoding, &(*mtf_tail), temp_char);

			lookup_length++;

		/* When the character is in the lookup list */
		} else {
		
			/* Using index of a specific char, create a copy of the node */
			index = get_index(*lookup_list, *buffer);
			temp_char = peek_at(*lookup_list, &(*lookup_tail), index);
			switch_char = new_charval(temp_char->c, NOT_DIGIT);

			/* Remove node from end of the list and add to front */
			*lookup_list = remove_at(*lookup_list, &(*lookup_tail), index);
			*lookup_list = add_front(*lookup_list, &(*lookup_tail), switch_char);

			/* Add the reference position to mtf list */
			temp_char = new_charval(index+1, IS_DIGIT);
			*mtf_encoding = add_end(*mtf_encoding, &(*mtf_tail), temp_char);

		}
	} 
}

void encode_rl(charval_t *mtf_encoding, charval_t **rl_encoding, charval_t **rl_tail){
/* Performs run-length encoding on a stream of input text by grouping
	repeating elements together. */

	charval_t *rl_char = mtf_encoding;
	int duplicate_counter = 0;
	int ones = 0;
	charval_t *temp_char = NULL;
	
	while(rl_char != NULL){
		
		/* Only check for repeating characters if the character 
			is a number and if the repeating value is 1 */
		if((rl_char->flag == 1) && rl_char->c == 1){

			/* The order is important here.  rl_char != NULL needs to evaluate first
				so that rl_char->val is not evaluated if its not true. */
			while((rl_char != NULL) && (rl_char->c == 1)){
				duplicate_counter++;		  
				rl_char = rl_char->next;
			}

			if(duplicate_counter >= 3){
		
				temp_char = new_charval(0, IS_DIGIT);
				*rl_encoding = add_end(*rl_encoding, &(*rl_tail), temp_char);
				
				temp_char = new_charval(duplicate_counter, IS_DIGIT);
				*rl_encoding = add_end(*rl_encoding, &(*rl_tail), temp_char);
			
			/* Recreate all the ones if there weren't at least three */
			} else {
				for(ones = 0; ones < duplicate_counter; ones++){
					temp_char = new_charval(1, IS_DIGIT);
					*rl_encoding = add_end(*rl_encoding, &(*rl_tail), temp_char);
				}
			}
		
			duplicate_counter = 0;
		
		} else {
			/* Case if the character is a number not equal to 1 */
			if(rl_char->flag == 1){
			
				temp_char = new_charval(rl_char->c, IS_DIGIT);
				*rl_encoding = add_end(*rl_encoding, &(*rl_tail), temp_char);
				rl_char = rl_char->next;

			/* Case if the character is not a number */
			} else {
				
				temp_char = new_charval(rl_char->c, NOT_DIGIT);
				*rl_encoding = add_end(*rl_encoding, &(*rl_tail), temp_char);
				rl_char = rl_char->next;

			}
		}
	}
}

void encode_ascii(charval_t *rl_encoding, \
						charval_t **ascii_encoding, charval_t **ascii_tail){
/* Performs a modified ascii encoding where characters 'flagged' as
	integers are store as their value + 128 */

	charval_t *temp_char = NULL;

	for( ; rl_encoding != NULL; rl_encoding=rl_encoding->next){
		if(rl_encoding->flag == 1){
			temp_char = new_charval((rl_encoding->c+128), IS_DIGIT);
			*ascii_encoding = add_end(*ascii_encoding, &(*ascii_tail), temp_char);
		} else {
			temp_char = new_charval(rl_encoding->c, NOT_DIGIT);
			*ascii_encoding = add_end(*ascii_encoding, &(*ascii_tail), temp_char);
		}
	}
}

/* ----------------------------------------------------------------------- DECODE */
void decode(unsigned char *buffer){
/* Decodes a stream of text by decoding the modified ascii encoding, 
	decoding the run-length encoding, followed by decoding the move-to-front
	encoding */

	/* Create the list instances */
	charval_t *ascii_decode, *ascii_tail;
	charval_t *rl_decode, *rl_tail;
	charval_t *mtf_decode, *mtf_tail;

	/* Initialze all pointers to NULL */
	ascii_decode = ascii_tail = NULL;
	rl_decode = rl_tail = NULL;
	mtf_decode = mtf_tail = NULL;

	decode_ascii(buffer, &ascii_decode, &ascii_tail);
	decode_rl(ascii_decode, &rl_decode, &rl_tail);
	decode_mtf(rl_decode, &mtf_decode, &mtf_tail);
	write_to_file(mtf_decode);

	/* Delete all lists to avoid any memory leaks */
	delete_list(ascii_decode, &ascii_tail);
	delete_list(rl_decode, &rl_tail);
	delete_list(mtf_decode, &mtf_tail);

}

void decode_ascii(unsigned char *buffer, \
						charval_t **ascii_decode, charval_t **ascii_tail){
/* Decodes an incoming stream of text by taking all characters whose
	value is greater than 128 and subtracting 128 to be left with 
	the original integer value */

	charval_t *temp_char;

	for( ; *buffer; buffer++){
		if(*buffer >= 128){
			temp_char = new_charval((*buffer-128), IS_DIGIT);
			*ascii_decode = add_end(*ascii_decode, &(*ascii_tail), temp_char);
		} else {
			temp_char = new_charval(*buffer, NOT_DIGIT);
			*ascii_decode = add_end(*ascii_decode, &(*ascii_tail), temp_char);
		}
	}

}

void decode_rl(charval_t *ascii_decode, \
					charval_t **rl_decode, charval_t **rl_tail){
/* Decodes the run length encoding by locating 0s in the list 
	and recreating the number of 1s specified after the 0 */
	
	int num_dups, dups_count;
	charval_t *temp_char;

	while(ascii_decode != NULL){
		
		/* Only add 1s if the character is an int whose value is 0 */
		if(ascii_decode->flag==1 && ascii_decode->c==0){
			ascii_decode = ascii_decode->next;
			num_dups = ascii_decode->c;

			for(dups_count=0; dups_count < num_dups; dups_count++){
				temp_char = new_charval(1, IS_DIGIT);
				*rl_decode = add_end(*rl_decode, &(*rl_tail), temp_char);
			}

		} else {
			/* Case when the character is a digit */
			if(ascii_decode->flag==1){
				temp_char = new_charval(ascii_decode->c, IS_DIGIT);
				*rl_decode = add_end(*rl_decode, &(*rl_tail), temp_char);
		
			/* Case when the character is not a digit */
			} else {
				temp_char = new_charval(ascii_decode->c, NOT_DIGIT);
				*rl_decode = add_end(*rl_decode, &(*rl_tail), temp_char);
			}
		}
		ascii_decode = ascii_decode->next;
	}

}

void decode_mtf(charval_t *rl_decoding, \
					charval_t **mtf_decoding, charval_t **mtf_tail){
/* Performs mtf-decoding by recreating a look up list and 
	subsequently pulling from the list with specified indices */

	charval_t *lookup_list, *lookup_tail;
	lookup_list = lookup_tail = NULL;
	
	charval_t *lookup_char = NULL;
	charval_t *decoded_char = NULL;
	charval_t *move_char = NULL;

	int index = 0;

	while(rl_decoding != NULL){

		/* If a character immediately follows a digit, it means its not
			already in the look up list, therefore, need to add to
			look up list */
		if((rl_decoding->flag == 1) && (rl_decoding->next != NULL)){
			
			if(rl_decoding->next->flag != 1){

				lookup_char = new_charval(rl_decoding->next->c, NOT_DIGIT);
				decoded_char = new_charval(rl_decoding->next->c, NOT_DIGIT);

				lookup_list = add_end(lookup_list, &lookup_tail, lookup_char);
				*mtf_decoding = add_end(*mtf_decoding, &(*mtf_tail), decoded_char);
				
				/* Skip over the character just added to the lookup list */
				rl_decoding = rl_decoding->next;

			} else {

				index = (rl_decoding->c)-1;
				lookup_char = peek_at(lookup_list, &lookup_tail, index);
				
				move_char = new_charval(lookup_char->c, NOT_DIGIT);
				decoded_char = new_charval(lookup_char->c, NOT_DIGIT);

				/* Anytime a character is used, pull it from the list and 
					place it at the top of the list */
				lookup_list = remove_at(lookup_list, &lookup_tail, index);
				lookup_list = add_front(lookup_list, &lookup_tail, move_char);
				*mtf_decoding = add_end(*mtf_decoding, &(*mtf_tail), decoded_char);
			
			}

		/* Edge case for last character in list */
		} else {

			index = (rl_decoding->c)-1;
			lookup_char = peek_at(lookup_list, &lookup_tail, index);
				
			move_char = new_charval(lookup_char->c, NOT_DIGIT);
			decoded_char = new_charval(lookup_char->c, NOT_DIGIT);

			lookup_list = remove_at(lookup_list, &lookup_tail, index);
			lookup_list = add_front(lookup_list, &lookup_tail, move_char);
			*mtf_decoding = add_end(*mtf_decoding, &(*mtf_tail), decoded_char);

		}

		rl_decoding = rl_decoding->next;
	}
	
	delete_list(lookup_list, &lookup_tail);
}
