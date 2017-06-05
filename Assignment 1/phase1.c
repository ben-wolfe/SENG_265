#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 22 /* String arrays should hold 20 characters.  Include +1
                    for the end of text character and +1 for the NULL character */
#define NAME_MAX 61
#define INFO_BYTES 8
#define FORWARD 1
#define BACKWARD 2

char comp_string[BUFFER], uncomp_string[BUFFER];
char compressed[BUFFER][BUFFER], uncompressed[BUFFER][BUFFER], uncompressed_temp[BUFFER][BUFFER];
char input_name[NAME_MAX], output_name[NAME_MAX];
FILE *input_file, *output_file;

int block_size = 20; /* By default, this value is at least 2 chars less than BUFFER.
                        This value is overwritten during the backwards direction
                        transformation depending on the compression scheme used. */

void read_command_arguments(int *argc, int *dir, int *input, int *output, char **argv);
void check_command_arguments(int* input, int* output, int* direction);
void write_info_bytes();
void read_info_bytes();
int compare_strings(const void *a, const void *b);
void compress_data();
void uncompress_data();

/* --------------------------------------------------------------------- MAIN */
int main (int argc, char *argv[]){

  int direction, input_status, output_status;
  direction = input_status = output_status = 0;
  input_file = output_file = NULL;

  /* Get and check command line arguments */
  read_command_arguments(&argc, &direction, &input_status, &output_status, argv);
  check_command_arguments(&input_status, &output_status, &direction);

  /* Reaching here means successful argument check - attempt to open files */
  input_file = fopen(input_name, "r");

  if(input_file == NULL){
    fprintf(stderr, "\nFILE ERROR\nUnable to open file: '%s'...\n", input_name);
    printf("Program terminated.\n\n");
    exit(1);
  }

  output_file = fopen(output_name, "w"); /* Automatic generation - no checking required */

  /* Clear the contents of uncomp_string and comp_string before manipulation */
  strncpy(uncomp_string,"",BUFFER);
  strncpy(comp_string, "", BUFFER);

  switch (direction) {
    case FORWARD:
      compress_data();
      break;
    case BACKWARD:
      uncompress_data();
      break;
    default:
      printf("\nPROGRAM ERROR\nProgram terminated.\n\n");
      exit(1);
  }

  /* Close files */
  fclose(input_file);
  fclose(output_file);

  return(0);
}

/* ---------------------------------------------------------------- FUNCTIONS */
void read_command_arguments(int *argc, int *dir, int *input, int *output, char **argv){

  int arg;

  for(arg=0; arg < *argc; arg++){

    if(!strcmp("--infile", argv[arg])){
      if(arg != *argc-1){
        strncpy(input_name, argv[arg+1], NAME_MAX);
        *input = 1; /* Indicates the user identified an input file */
      } else {
        printf("\nCOMMAND LINE ERROR\nPlease include a file name after '--infile'.\n\n");
        exit(1);
      }
    }

    if(!strcmp("--outfile", argv[arg])){
      if(arg != *argc-1){
        strncpy(output_name, argv[arg+1], NAME_MAX);
        *output = 1; /* Indicates the user identified an output file */
      } else {
        printf("\nCOMMAND LINE ERROR\nPlease include a file name after '--outfile'.\n\n");
        exit(1);
      }
    }

    if(!strcmp("--forward", argv[arg])){
      *dir = FORWARD;
    }
    if(!strcmp("--backward", argv[arg])){
      *dir = BACKWARD;
    }
  }

  return;
}

void check_command_arguments(int *input, int *output, int *direction){

  if((!*input) || (!*output) || (!*direction)){

    printf("\nCOMMAND LINE ERROR\nPlease include the following arguments: \n");

    if(!*input) printf("   An input file: \"--infile filename.txt\".\n");
    if(!*output) printf("   An output file: \"--outfile filename.txt\".\n");
    if(!*direction) printf("   A transformation direction: Either \"--forward\" or \"--backward\"\n");

    printf("\n");
    exit(1);
  }

  return;
} /* End argument_error_checking */

void write_info_bytes(){

  fputc('\xab', output_file);
  fputc('\xba', output_file);
  fputc('\xbe', output_file);
  fputc('\xef', output_file);
  fputc(block_size, output_file);
  fputc(0, output_file);
  fputc(0, output_file);
  fputc(0, output_file);

  return;
}

void compress_data(){
  int string_length, i, j;
  char ETX_char = '\x03';

  write_info_bytes();

  /* Transfer and manipulate file contents */
  while(fread(&uncomp_string, sizeof(char), block_size, input_file) > 0){

    string_length = strlen(uncomp_string); /* Get string length */
    uncomp_string[string_length] = ETX_char; /* Add end of text character */
    string_length = strlen(uncomp_string); /* Update string_length */

    for(i=0; i<string_length; i++) compressed[i][0] = uncomp_string[i];

    /* Rotate array elements */
    for(j = 0; j<string_length-1; j++){
      for(i = 0; i<string_length; i++){
        if(i == 0) {
          compressed[string_length-1][j+1] = compressed[i][j];
        } else {
          compressed[i-1][j+1] = compressed[i][j];
        }
      }
    }

    /* Add NULL terminator to each row of rotated string */
    for(i=0; i<string_length; i++) compressed[i][string_length] = '\x0';

    /* Sort the array in lexicographic order */
    qsort(compressed, string_length, sizeof(char)*(BUFFER), compare_strings);

    /*printf("Last compressed string written...\n");*/
    for(i=0; i<string_length; i++){
      comp_string[i] = compressed[i][string_length-1];
    }

    /* Write the last column to a string */
    fwrite(comp_string, sizeof(char), string_length, output_file);

    /* Clear the array and string for the next data stream */
    memset(compressed, 0, sizeof(compressed[0][0]) * BUFFER * BUFFER);
    strncpy(uncomp_string, "" ,BUFFER);
    strncpy(comp_string, "" ,BUFFER);
  }

  return;
}

void read_info_bytes(){

  int i = 0;

  /* Check for incorrect file types */
  for(i=0; i<INFO_BYTES; i++){
    comp_string[i] = fgetc(input_file);

    if(i == 3){
      if(comp_string[0] != '\xab' || comp_string[1] != '\xba'){
        printf("\nFILE ERROR\n'%s' is not a .ph1 file...\nProgram terminated.\n\n", input_name);
        exit(1);
      }

      if(comp_string[2] != '\xbe' || comp_string[3] != '\xef'){
        printf("\nFILE ERROR\n'%s' is not a .ph1 file...\nProgram terminated.\n\n", input_name);
        exit(1);
      }
    }

    if(i == 4){
      if(comp_string[4] <= 20){

        block_size = comp_string[4];

      } else {

        printf("\nPROGRAM ERROR\nIncompatible compression schemes. File '%s' was generated using blocks longer than 20 characters...\nProgram terminated.\n\n", input_name);
        exit(1);

      }
    }
  }

  /* clear contents of comp_string */
  strncpy(comp_string, "", BUFFER);

  return;
}

void uncompress_data(){

  int row, col, temp_col, string_length, key_row;
  row = col = temp_col = string_length = key_row = 0;
  int ETX_count = 0;

  read_info_bytes();

  while(fread(&comp_string, sizeof(char), block_size+1, input_file) > 0){
    string_length = strlen(comp_string);

    /* Check to make sure the text is properly formatted */
    ETX_count = 0;
    for(col=0; col<string_length; col++){
      if(comp_string[col] == '\x03') ETX_count++;
    }

    if(ETX_count != 1){
      printf("\nPROGRAM ERROR\nIncompatible compression schemes. '%s' was not generated using %d character blocks as the first 8 bytes indicate...\nProgram terminated.\n\n", input_name, block_size);
      exit(1);
    }
    ETX_count = 0;

    /* Store the contents of the string vertically */
    for(row=0; row<string_length; row++) uncompressed[row][0] = comp_string[row];

    /* Perform array transformation */
    for(col=0; col<string_length-1; col++){

      /* Copy the conents of the originial string to a temporary array */
      for(temp_col=0; temp_col<=col; temp_col++){
        for(row=0; row<string_length; row++){
          uncompressed_temp[row][temp_col] = uncompressed[row][temp_col];
        }
      }

      /* Sort the rows of the temporary array in lexographic order */
      qsort(uncompressed_temp, string_length, sizeof(char)*(BUFFER), compare_strings);

      /* Write the contents of the temporary array back to the oringial array */
      for(row=0; row<string_length; row++) uncompressed[row][col+1] = uncompressed_temp[row][col];

    }

    /* Search for ETX symbol in the last column */
    for(row=0; row<string_length; row++){
      if(uncompressed[row][string_length-1] == '\x03'){
        key_row = row;
      }
    }

    /* Write the string stored key row to uncomp_string */
    for(col=0; col<string_length; col++){
      uncomp_string[col] = uncompressed[key_row][col];
    }
    /* Add NULL terminator to string*/
    uncomp_string[string_length] = '\x0';

    /* Write the uncompressed string to output file  */
    fwrite(uncomp_string, sizeof(char), string_length-1, output_file);

    /* Clear the array and string for the next data stream */
    memset(uncompressed, 0, sizeof(uncompressed[0][0]) * BUFFER * BUFFER);
    memset(uncompressed_temp, 0, sizeof(uncompressed_temp[0][0]) * BUFFER * BUFFER);
    strncpy(uncomp_string, "" ,BUFFER);
    strncpy(comp_string, "" ,BUFFER);

  }

  return;
}

int compare_strings(const void *a, const void *b){
  char *sa = (char *)a;
  char *sb = (char *)b;

  return strcmp(sa, sb);
}
