#!/usr/bin/env python3

"""

B.Wolfe | V00205547

DESCRIPTION: 
This program compresses and extracts text files using the Burrows-Wheeler transform,
move-to-front encoding, and run-length encoding in block sizes up to 500 characters.

DATE:
11.07.2017

"""


import sys
import argparse
import struct
from itertools import groupby
import re


default_blocksize = 500
ph2_magic = '\xda\xaa\xaa\xad'


def compress(infile, outfile, blocksize):
	""" 
	Compress performs a forward text transformation then calls the required 
	functions for move-to-front encoding and run-length encoding
	"""
	
	write_magic_bytes(outfile, blocksize, ph2_magic)
	transformed_text = []

	# Attempt to open 'infile'; exit gracefully if opening is unccessful
	try:
		with open(infile, 'r', encoding='latin-1') as input_fp:
			# Read the file block by block while there is something to read
			text_buffer = input_fp.read(blocksize)
			while text_buffer:
				buffer_mod = text_buffer + '\x03'

				# Store word rotations in a list
				word_list = [buffer_mod[x:]+buffer_mod[:x] for x in range(len(buffer_mod))]
			
				# Sort list; extract last character from each word; join to create the compressed word
				word_list.sort()
				new_word = "".join([word[-1] for word in word_list])

				transformed_text.extend(new_word)

				# Read in the next stream of text
				text_buffer = input_fp.read(blocksize)	
	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)

	transformed_text = "".join(transformed_text)
	encode(transformed_text, blocksize, outfile)



def write_magic_bytes(outfile, blocksize, magic_bytes):
	"""
	Writes the first 4 file-identifying bytes to the file followed by the blocksize.
	"""
	# Include the identifying file bytes to file_details
	file_details = magic_bytes

	# Convert the blocksize to a string to store in file_details 
	num_bytes = struct.pack("I", blocksize)
	char_bytes = [chr(n) for n in num_bytes]
	file_details += "".join(char_bytes)

	# Write the file details as the first 8 bytes of the file
	with open(outfile, 'w', encoding='latin-1') as output_fp:
		output_fp.write(file_details)



def encode(transformed_text, blocksize, outfile):
	""" 
	Encodes the contents of an input file using move-to-front encoding followed 
	by run-length encoding and converts the result to an ascii string using a
	modified scheme for numeric characters.
	"""
	
	mtf = mtf_encode(transformed_text)
	rl = rl_encode(mtf)
	anscii = to_anscii(rl)

	# Attempt to open and append to 'outfile'; exit gracefully if opening is unccessful
	try:
		with open(outfile, 'a', encoding='latin-1') as output_file:
			output_file.write(anscii)

	except IOError:
		print('\nFILE ERROR\nUnable to write to %s\n' % outfile)
		sys.exit()



def mtf_encode(buf):
	"""
	Performs move-to-front encoding which appends every new character to a list
	and references the position for every already-seen character in the buffer.
	"""
	
	lookup_list = []
	encoding = [] 
    
	for char in buf:
		
		if char not in lookup_list:
			lookup_list.append(char)
			# Append the new character and its list position to the encoding list
			encoding.extend((len(lookup_list), char))
		
		elif char in lookup_list:
			index = lookup_list.index(char) # Get the character's index from the list
			encoding.append(index+1) # The +1 is because position indexing starts at 1
			lookup_list.pop(index) # Remove the character at index 'index'
			lookup_list.insert(0,char) # Reposition the character to the top of the list

	return encoding



def rl_encode(buf):
	"""
	Groups repated elements together and performs an aggregation when the repeated
	element is 1 and the number of repeats is 3 or greater.
	"""
    
	encoding = []
	# Group together like elements in the result from mtf encoding
	num_grouping = [list(group) for (_,group) in groupby(buf)]    
	for group in num_grouping:

		# Grouping should only be performed on runs of size 3 or greater and if the num is 1
		if len(group) >= 3 and group[0]==1:
			encoding.extend((0, len(group)))
		else:
			encoding.extend(group)

	return encoding




def to_anscii(buf):
	"""
	Converts a string to its ascii equivalent using a modified scheme for numeric
	characters.
	"""    
	anscii_list = []
	anscii_string = ""
    
	for char in buf:
		if isinstance(char, int):
			anscii_char = chr(char+128) # An assumption is made here that the text file only
												 # contains characters of ascii characters 127 or less.
			anscii_list.append(anscii_char)
		else:
			anscii_list.append(char)
    
	anscii_string = "".join(anscii_list)
	return anscii_string




def extract(infile, outfile):
	"""
	Decodes the contents of a .ph2 file by first converting the original string using 
	the custom anscii scheme identified above. Run length decode and move to front decode
	are then performed and the result is written to the output file. 
	"""

	blocksize = read_ph2_bytes(infile)

	# Attempt to open 'infile'; Exit gracefully if uncessful
	try:
		with open(infile, 'r', encoding='latin-1') as input_file:
			# Read the file as one block starting after the file details
			input_file.seek(8)
			buffer = input_file.read()

	except IOError:
		print("\nFILE ERROR\nUnable to open %s\n" % infile)
		sys.exit()

	alpha_num = from_anscii(buffer) # The results from ascii contain alphabetic and numeric chars
	rl = rl_decode(alpha_num)
	rotated_text = mtf_decode(rl)

	reverse_transform(rotated_text, blocksize, outfile)



def read_ph2_bytes(infile):
	"""
	Reads the first 8 bytes of the text file to 1) confirm the file has the
	4 identifying ph2 bytes and 2) to obtain the block size.
	"""
	try:
		with open(infile, 'r', encoding='latin-1') as input_fp:
			f_info = list(input_fp.read(8))

			# Check that the input file is of .ph1 file type
			if f_info[0] != '\xda' or f_info[1] != '\xaa' or f_info[2] != '\xaa' or f_info[3] != '\xad':
				print('\nFILE ERROR\nInput file is not a .ph2 file\n')
				sys.exit();
	
			to_ints = [ord(num) for num in f_info[4:]]
			to_bytes = bytearray(to_ints)
			blocksize = struct.unpack("I", to_bytes)
			blocksize = blocksize[0]
			
	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)
		sys.exit()

	return blocksize




def from_anscii(buf):
	"""
	Converts a string to its alpha-numeric equivalent using a modified scheme for numeric
	characters and stores the results in a list.
	"""

	alpha_num_list = []
    
	for char in buf:
		if ord(char) >= 128:
			num = ord(char)-128 # Subtract 128 to obtain original numeric charccters (based
									  # on our custom encoding scheme used for numeric characters)
			alpha_num_list.append(num)
		
		else:
			alpha_num_list.append(char)

	return alpha_num_list




def rl_decode(buf):
	"""
	Uncompresses the buffer by adding the correct number of 1s identified by the digit after 0
	(since 0 indicates a repeated character when the original compression is performed).
	"""
	
	decoding = []
    
	for index, char in enumerate(buf):
		if char == 0:
			for repeats in range(buf[index+1]):
				decoding.append(1)
		else:
			if buf[index-1] != 0:
				decoding.append(char)
    
	return decoding



def mtf_decode(buf):
	"""
	Decodes the buffer by appending every new character instance to the look up list then by
	using this look up list to decode the remaining string of digits (while writing the decoded 
	characters to a new list).
	"""
    
	lookup_list = []
	decoding = []
	decoded_str = ""
    
	for char in buf:
		if isinstance(char,int):
			# Ignore the numbers that are less than the length of the list.  It indicates
			# a new character character is the next element in the string.
			if char > len(lookup_list):
				continue
				
			# If the number is less than the length of the look-up list, it means we're
			# trying to decode a character.
			else:
				decoded_char = lookup_list.pop(char-1) # Subtract 1 to account for the list
					 												# indexing scheme starting at 1
				decoding.append(decoded_char) # Add the decoded character to our list
				lookup_list.insert(0, decoded_char) # Move the reference char to the top of the list
        
		elif isinstance(char,str):
			# Add the character to the decoding list as well as the lookup list for further
			# decoding.
			lookup_list.append(char)
			decoding.append(char)
    
	decoded_str = "".join(decoding)
	return decoded_str



def reverse_transform(rotated_text, blocksize, outfile):
	"""
	Performs the reverse text transformation to get back to the original text file.
	"""
	
	# Clear the contents of the output file (preventative measure if the file already exists)
	with open(outfile, 'w', encoding='latin-1') as output_fp:
		output_fp.write("")
				
	# Read the text 'blocksize+1' characters at a time to account for the '\x03' symbol
	marker = 0
	while marker < len(rotated_text):
		
		# Update the text buffer
		original = list(rotated_text[marker : (marker+blocksize+1)])
		transfer_counter = 0
		
		while transfer_counter < (len(original)-1):	
			# Copy the contents of the original list to a temporary list and sort
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

		# Update the start of string marker 
		marker += blocksize+1



def check_arguments(args):
	"""
	Checks the input file names against regular expressions to make sure the extensions are
	.txt and .ph2.
	"""

	if args.compress:
		f_in_pat = r'.+.txt\b'
		f_out_pat = r'.+.ph2\b'
		in_ext = '.txt'
		out_ext = '.ph2'

	elif args.extract:
		f_in_pat = r'.+.ph2\b'
		f_out_pat = r'.+.txt\b'
		in_ext = '.ph2'
		out_ext = '.txt'

	if not re.match(f_in_pat, args.infile) or not re.match(f_out_pat, args.outfile):
		print("\nFILE ERROR\nThe following files have invalid extensions:")
		
		if not re.match(f_in_pat, args.infile):
			print("- %s (Input file must have a %s extension)" % (args.infile, in_ext))
			
		if not re.match(f_out_pat, args.outfile):
			print("- %s (Output file must have a %s extension)" % (args.outfile, out_ext))

		print()
		sys.exit()


def read_command_line():
	# Create the argument parser object
	parser = argparse.ArgumentParser(description='A program for text compression and extraction.')

	# Specify mutually exclusive arguments
	coding = parser.add_mutually_exclusive_group(required=True)
	coding.add_argument('--compress', action='store_true', help='specify for compression')
	coding.add_argument('--extract', action='store_true', help='specify for extraction')

	# Specify additional arguments
	parser.add_argument('--infile', type=str, help='input text file', required=True)
	parser.add_argument('--outfile', type=str, help='compressed output file', required=True)
	parser.add_argument('--blocksize', type=int, help='select the blocksize for compression')

	# Parse and check the arguments
	args = parser.parse_args()
	check_arguments(args)
	
	# Use transformation direction to select program mode 
	if args.compress:
		# Use the user specified blocksize otherwise use the default
		if args.blocksize:
			blocksize = args.blocksize
		else:
			blocksize = default_blocksize
		compress(args.infile, args.outfile, blocksize)
		return

	elif args.extract:
		extract(args.infile, args.outfile)
		return
	
	else:
		sys.exit()


def main():
	read_command_line()


if __name__ == '__main__':
	main()
