#!/usr/bin/env python3

"""
B.Wolfe | V00205547
SENG 265 - Assignment 2

Description: This program is phase 2 of a text compression scheme.  The program
transforms blocks of text forwards and backwards using any user specified block 
size.  Adhering to good Python practice, list comprehensions are used instead of 
for loops.  The program checks for the correct .ph1 file format and implements 
command line argument checking on top of the default argparse error checking to 
ensure correct program usage.
"""

import sys
import argparse
import struct

def compress_text(infile, outfile, block):
	# Call a separate function to write file detail bytes
	write_key_bytes(outfile, block)

	# Attempt to open 'infile'; exit gracefully if opening is unccessful
	try:
		with open(infile, 'r') as input_fp:
			# Read the file block by block while there is something to read
			text_buffer = input_fp.read(block)
			while text_buffer:
				buffer_mod = text_buffer + '\x03'

				# Store word rotations in a list
				word_list = [buffer_mod[x:]+buffer_mod[:x] for x in range(len(buffer_mod))]
			
				# Sort list; extract last character from each word; join to create the compressed word
				word_list.sort()
				new_word = "".join([word[-1] for word in word_list])
				
				# Append the compressed word to output file
				with open(outfile, 'a', encoding='latin-1') as output_fp:
					output_fp.write(new_word)

				# Read in the next stream of text
				text_buffer = input_fp.read(block)	
	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)

def write_key_bytes(outfile, blocksize):
	# Include the identifying 'ph1' file bytes to file_details
	file_details = '\xab\xba\xbe\xef'

	# Convert the blocksize to a string to store in file_details 
	num_bytes = struct.pack("I", blocksize)
	char_bytes = [chr(n) for n in num_bytes]
	file_details += "".join(char_bytes)

	# Write the file details as the first 8 bytes of the file
	with open(outfile, 'w', encoding='latin-1') as output_fp:
		output_fp.write(file_details)

def uncompress_text(infile, outfile):
	blocksize = read_key_bytes(infile)
	try:
		with open(infile, 'r') as input_fp:
			# Start reading the file after the file detail bytes
			input_fp.seek(8)

			# Clear the contents of the output file (preventative measure if the file already exists)
			with open(outfile, 'w', encoding='latin-1') as output_fp:
				output_fp.write("")
				
			# Read the text 'blocksize+1' characters at a time to account for the '\x03' symbol
			original = list(input_fp.read(blocksize+1))
			while original:
	
				transfer_counter = 0
				while transfer_counter < (len(original)-1):
					# Copy the contents of the original list to a temporary and sort
					temporary = list(original)
					temporary.sort()

					# Add last char of each element in the temp list to original list
					original = [original[x]+temporary[x][-1] for x in range(len(original))]
					transfer_counter += 1

				# Search for EOT symbol and retrieve corresponding word
				uncomp_string = [original[x][:-1] for x in range(len(original)) if original[x][-1] == '\x03']

				# Append the contents to outfile
				with open(outfile, 'a', encoding='latin-1') as output_fp:
					output_fp.write(uncomp_string[0])

				# Update the text buffer
				original = list(input_fp.read(blocksize+1))
				
	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)
		return

def read_key_bytes(infile):
	try:
		with open(infile, 'r', encoding='latin-1') as input_fp:
			f_info = list(input_fp.read(8))

			# Check that the input file is of .ph1 file type
			if f_info[0] != '\xab' or f_info[1] != '\xba' or f_info[2] != '\xbe' or f_info[3] != '\xef':
				print('\nFILE ERROR\nInput file is not a .ph1 file\n')
				return;

			to_ints = [ord(num) for num in f_info[4:]]
			to_bytes = bytearray(to_ints)
			blocksize = struct.unpack("I", to_bytes)
			blocksize = blocksize[0]
			
	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)
		return

	return blocksize

def read_command_line():
	# Create the argument parser object
	parser = argparse.ArgumentParser(description='A program for text compression and extraction.')

	# Specify mutually exclusive arguments
	direction = parser.add_mutually_exclusive_group(required=True)
	direction.add_argument('--forward', action='store_true', help='specify for compression')
	direction.add_argument('--backward', action='store_true', help='specify for extraction')

	# Specify additional arguments
	parser.add_argument('--infile', type=str, help='input text file', required=True)
	parser.add_argument('--outfile', type=str, help='stores program output', required=True)
	parser.add_argument('--blocksize', type=int, help='for use with forward transform; number of characters per compression block')
	
	# Parse and check the arguments
	args = parser.parse_args()
	check_arguments(args.infile, args.outfile, args.blocksize, args.forward, args.backward)
	
	# Use transformation direction to select program mode 
	if args.forward:
		compress_text(args.infile, args.outfile, args.blocksize)
		return

	elif args.backward:
		uncompress_text(args.infile, args.outfile)
		return
	
	else:
		sys.exit()

def check_arguments(infile, outfile, blocksize, forward, backward):

	if forward and (infile[-4:] != '.txt'):
		print("\nINPUT ERROR\nInput file must be of type '.txt' to perform the forward transform\n")
		sys.exit()

	if backward and (infile[-4:] != '.ph1'):
		print("\nINPUT ERROR\nInput file must of type '.ph1' to perform the backward transform\n")
		sys.exit()

	# Check that a block size is specified in the forward direction
	if forward and blocksize == None:
		print("\nINPUT ERROR\nInclude a block size to use in the forward transform: '--blocksize <#>'\n")
		sys.exit()

def main():
	read_command_line()

if __name__ == '__main__':
		  main()
