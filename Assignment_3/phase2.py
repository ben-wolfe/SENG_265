#!/usr/bin/env python3

"""

B.Wolfe | V00205547
SENG 265 - Assignment 3

DESCRIPTION: 
This program is phase 2 of a text compression scheme.  The program
performs move-to-front encoding followed by run-length encoding to
compress a text file.

DATE:
11.07.2017

"""

import sys
import argparse
import struct
from itertools import groupby
import re

ph1_magic = '\xab\xba\xbe\xef'
ph2_magic = '\xda\xaa\xaa\xad'

def encode(infile, outfile):
	""" 
	Encodes the contents of an input file using move-to-front encoding followed 
	by run-length encoding and converts the result to an ascii string using a
	modified scheme for numeric characters.
	"""
	blocksize = read_ph1_bytes(infile)
	write_magic_bytes(outfile, blocksize, ph2_magic)

	# Attempt to open 'infile'; exit gracefully if opening is unccessful
	try:
		with open(infile, 'r', encoding='latin-1') as input_file:
			# Read the file as one block starting after the file details
			input_file.seek(8)
			buffer = input_file.read()

	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)
		sys.exit()

	mtf = mtf_encode(buffer)
	rl = rl_encode(mtf)
	anscii = to_anscii(rl)

	# Attempt to open and append to 'outfile'; exit gracefully if opening is unccessful
	try:
		with open(outfile, 'a', encoding='latin-1') as output_file:
			output_file.write(anscii)

	except IOError:
		print('\nFILE ERROR\nUnable to write to %s\n' % outfile)
		sys.exit()

def read_ph1_bytes(infile):
	"""
	Reads the first 8 bytes of the text file to 1) confirm the file has the
	4 identifying ph1 bytes and 2) to obtain the block size.
	"""
	try:
		with open(infile, 'r', encoding='latin-1') as input_fp:
			f_info = list(input_fp.read(8))

			# Check that the input file is of .ph1 file type
			if f_info[0] != '\xab' or f_info[1] != '\xba' or f_info[2] != '\xbe' or f_info[3] != '\xef':
				print('\nFILE ERROR\nInput file is not a .ph1 file\n')
				sys.exit();
			
			# Aggregate the 4 number bytes to obtain the block size
			to_ints = [ord(num) for num in f_info[4:]]
			to_bytes = bytearray(to_ints)
			blocksize = struct.unpack("I", to_bytes)
			blocksize = blocksize[0]
			
	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)
		sys.exit()

	return blocksize

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

def decode(infile, outfile):
	"""
	Decodes the contents of a .ph2 file by first converting the original string using 
	the custom anscii scheme identified above. Run length decode and move to front decode
	are then performed and the result is written to the output file. 
	"""

	blocksize = read_ph2_bytes(infile)
	write_magic_bytes(outfile, blocksize, ph1_magic)

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
	mtf = mtf_decode(rl)
	
	try:
		with open(outfile, 'a', encoding='latin-1') as output_file:
			output_file.write(mtf)
	except IOError:
		print("\nFILE ERROR\nUnable to open %s\n" %outfile)
		sys.exit()

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

def read_command_line():
	# Create the argument parser object
	parser = argparse.ArgumentParser(description='A program for text compression and extraction.')

	# Specify mutually exclusive arguments
	coding = parser.add_mutually_exclusive_group(required=True)
	coding.add_argument('--encode', action='store_true', help='specify for compression')
	coding.add_argument('--decode', action='store_true', help='specify for extraction')

	# Specify additional arguments
	parser.add_argument('--infile', type=str, help='input text file', required=True)
	parser.add_argument('--outfile', type=str, help='stores program output', required=True)

	# Parse and check the arguments
	args = parser.parse_args()
	check_arguments(args)
	
	# Use transformation direction to select program mode 
	if args.encode:
		encode(args.infile, args.outfile)
		return

	elif args.decode:
		decode(args.infile, args.outfile)
		return
	
	else:
		sys.exit()

def check_arguments(args):
	"""
	Checks the input file names against regular expressions to make sure the extensions are
	.ph1 and .ph2.
	"""

	if args.encode:
		f_in_pat = r'.+.ph1\b'
		f_out_pat = r'.+.ph2\b'
		in_ext = '.ph1'
		out_ext = '.ph2'

	elif args.decode:
		f_in_pat = r'.+.ph2\b'
		f_out_pat = r'.+.ph1\b'
		in_ext = '.ph2'
		out_ext = '.ph1'

	if not re.match(f_in_pat, args.infile) or not re.match(f_out_pat, args.outfile):
		print("\nFILE ERROR\nThe following files have invalid extensions:")
		
		if not re.match(f_in_pat, args.infile):
			print("- %s (Input file must have a %s extension)" % (args.infile, in_ext))
			
		if not re.match(f_out_pat, args.outfile):
			print("- %s (Output file must have a %s extension)" % (args.outfile, out_ext))

		print()
		sys.exit()

def main():
	read_command_line()

if __name__ == '__main__':
	main()
