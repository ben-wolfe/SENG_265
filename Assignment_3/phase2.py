#!/usr/bin/env python3

"""

B.Wolfe | V00205547
SENG 265 - Assignment 3

DESCRIPTION: 
This program is phase 2 of a text compression scheme.  The program
performs move-to-front encoding followed by run-length encoding to
compress a text file.

DATE:
05.07.2017

"""


import sys
import argparse
import struct
from itertools import groupby

ph1_magic = '\xab\xba\xbe\xef'
ph2_magic = '\xda\xaa\xaa\xad'

def encode(infile, outfile):
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

def decode(infile, outfile):
	blocksize = read_ph2_bytes(infile)
	write_magic_bytes(outfile, blocksize, ph1_magic)

	# Attempt to open 'infile
	try:
		with open(infile, 'r', encoding='latin-1') as input_file:
			# Read the file as one block starting after the file details
			input_file.seek(8)
			buffer = input_file.read()

	except IOError:
		print("\nFILE ERROR\nUnable to open %s\n" % infile)
		sys.exit()

	alpha_num = from_anscii(buffer)
	rl = rl_decode(alpha_num)
	mtf = mtf_decode(rl)
	

	try:
		with open(outfile, 'a', encoding='latin-1') as output_file:
			output_file.write(mtf)
	except IOError:
		print("\nFILE ERROR\nUnable to open %s\n" %outfile)
		sys.exit()

def mtf_encode(buf):
    
    lookup_list = []
    encoding = [] 
    
    for char in buf:
        
        if char not in lookup_list:
            lookup_list.append(char)
            encoding.extend((len(lookup_list), char))
            
        elif char in lookup_list:
            index = lookup_list.index(char)
            encoding.append(index+1)
            lookup_list.pop(index)
            lookup_list.insert(0,char)

    return encoding

def rl_encode(buf):
    
    encoding = []
    num_grouping = [list(group) for (_,group) in groupby(buf)]    
    for group in num_grouping:
        if len(group) >= 3 and group[0]==1:
            encoding.extend((0, len(group)))
        else:
            encoding.extend(group)
            
    return encoding

def to_anscii(buf):
    
    anscii_list = []
    anscii_string = ""
    
    for char in buf:
        if isinstance(char, int):
            anscii_char = chr(char+128)
            anscii_list.append(anscii_char)
        else:
            anscii_list.append(char)
    
    anscii_string = "".join(anscii_list)
    return anscii_string

def mtf_decode(buf):
    
    lookup_list = []
    decoding = []
    decoded_str = ""
    
    for char in buf:
        
        if isinstance(char,int):
            if char > len(lookup_list):
                continue
            else:
                decoded_char = lookup_list.pop(char-1)
                decoding.append(decoded_char)
                lookup_list.insert(0, decoded_char)
        elif isinstance(char,str):
            lookup_list.append(char)
            decoding.append(char)
    
    decoded_str = "".join(decoding)
    return decoded_str

def rl_decode(buf):
    
    decoding = []
    
    for index, char in enumerate(buf):
        if char == 0:
            for repeats in range(buf[index+1]):
                decoding.append(1)
        else:
            if buf[index-1] != 0:
                decoding.append(char)
    
    return decoding

def from_anscii(buf):
    
    alpha_num_list = []
    
    for char in buf:
        if ord(char) >= 128:
            num = ord(char)-128
            alpha_num_list.append(num)
        else:
            alpha_num_list.append(char)
    
    return alpha_num_list

def read_ph2_bytes(infile):
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

def read_ph1_bytes(infile):
	try:
		with open(infile, 'r', encoding='latin-1') as input_fp:
			f_info = list(input_fp.read(8))

			# Check that the input file is of .ph1 file type
			if f_info[0] != '\xab' or f_info[1] != '\xba' or f_info[2] != '\xbe' or f_info[3] != '\xef':
				print('\nFILE ERROR\nInput file is not a .ph1 file\n')
				sys.exit();

			to_ints = [ord(num) for num in f_info[4:]]
			to_bytes = bytearray(to_ints)
			blocksize = struct.unpack("I", to_bytes)
			blocksize = blocksize[0]
			
	except IOError:
		print('\nFILE ERROR\nUnable to open %s\n' % infile)
		sys.exit()

	return blocksize

def write_magic_bytes(outfile, blocksize, magic_bytes):
	# Include the identifying 'ph1' file bytes to file_details
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
	#check_arguments(*args)
	
	# Use transformation direction to select program mode 
	if args.encode:
		encode(args.infile, args.outfile)
		return

	elif args.decode:
		decode(args.infile, args.outfile)
		return
	
	else:
		sys.exit()

def main():
	read_command_line()

if __name__ == '__main__':
	main()
