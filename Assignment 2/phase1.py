#!/usr/bin/env python3

import sys
import argparse
import struct

def compress_text(infile, outfile, block):
	# Call a separate function to write file detail bytes
	write_key_bytes(outfile, block)

	word_list = []
	# Attempt to open 'infile'; exit gracefully if opening is unccessful
	try:
		with open(infile, 'r') as input_fp:
			while True:
				# Read the file block by block
				text_buffer = input_fp.read(block)
				
				# Exit the loop when there is nothing left to read
				if not text_buffer:
					break
			
				buffer_mod = text_buffer + '\x03'

				# Store word rotations in a list
				for x in range(len(buffer_mod)):
					word_list.append(buffer_mod[x:]+buffer_mod[:x])
			
				# Sort the list; obtiain last character from each word; join
				# the characters to create the compressed word
				word_list.sort()
				new_word = [word[-1] for word in word_list]
				new_word = "".join(new_word)

				# Write the compressed word to output file
				with open(outfile, 'a', encoding='latin-1') as output_fp:
					output_fp.write(new_word)

				# Clear the word list for next transformation
				del word_list[:]
	
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
	original = []
	try:
		with open(infile, 'r') as input_fp:
			# Start reading the file after the file detail bytes
			input_fp.seek(8)

			# Clear the contents of the output file (preventative measure if 
			# the current file exists
			with open(outfile, 'w', encoding='latin-1') as output_fp:
				output_fp.write("")

			while True:
				# Read the next text stream 'blocksize+1' characters at a time
				# Note: the +1 is to account for the '\x03' symbol
				text_buffer = input_fp.read(blocksize+1)

				# Exit the loop if there is nothing left to read
				if not text_buffer:
					break;
	
				# Clear the last text buffer and update with new contents
				del original[:]
				original = list(text_buffer)

				# Outer loop for the number of transfers from original
				# list to the temporary list
				for loop in range(len(original)-1):
					# Copy the contents of the original list to a temporary
					# list; sort the temporary list
					temporary = list(original)
					temporary.sort()

					# Add the last char of each elemet in the temporary list
					# back to the original list
					for item in range(len(original)):
						original[item] += temporary[item][-1]

				# Search for EOT symbol and retrieve corresponding word
				for key_word in range(len(original)):
					if original[key_word][-1] == '\x03':
						uncomp_string = original[key_word]
						uncomp_string = uncomp_string[:-1]
						break

				# Append the contents to outfile
				with open(outfile, 'a', encoding='latin-1') as output_fp:
					output_fp.write(uncomp_string)

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
	direction = parser.add_mutually_exclusive_group()
	direction.add_argument('--forward', action='store_true', help='specify for compression')
	direction.add_argument('--backward', action='store_true', help='specify for extraction')

	# Specify additional arguments
	parser.add_argument('--infile', type=str, help='input text file')
	parser.add_argument('--outfile', type=str, help='stores program output')
	parser.add_argument('--blocksize', type=int, help='for use with forward transform; number of characters per compression block')
	
	# Parse the arguments
	args = parser.parse_args()
	
	# Check for valid program usage
	if args.infile == None or args.outfile == None:
		display_usage()
		return

	# Check that a block size is specified in the forward direction
	if args.forward and args.blocksize == None:
		display_usage()
		return

	# Use transformation direction to select program mode 
	if args.forward:
		compress_text(args.infile, args.outfile, args.blocksize)
		return

	elif args.backward:
		uncompress_text(args.infile, args.outfile)
		return
	
	else:
		display_usage()
		return

def display_usage():
	print('\nINPUT ERROR - Correct program usage:')
	print('Forward transform: ./phase1.py --foward --infile <file.txt> --outfile <file.ph1> --blocksize #')
	print('Backward transform: ./phase1.py --backward --infile <file.ph1> --outfile <file.txt>\n')

def main():
	read_command_line()

if __name__ == '__main__':
		  main()
