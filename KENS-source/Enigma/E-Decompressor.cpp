/*-----------------------------------------------------------------------------*\
|																				|
|	Enigma.dll: Compression / Decompression of data in Enigma format			|
|	Copyright � 2002-2004 The KENS Project Development Team						|
|																				|
|	This library is free software; you can redistribute it and/or				|
|	modify it under the terms of the GNU Lesser General Public					|
|	License as published by the Free Software Foundation; either				|
|	version 2.1 of the License, or (at your option) any later version.			|
|																				|
|	This library is distributed in the hope that it will be useful,				|
|	but WITHOUT ANY WARRANTY; without even the implied warranty of				|
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU			|
|	Lesser General Public License for more details.								|
|																				|
|	You should have received a copy of the GNU Lesser General Public			|
|	License along with this library; if not, write to the Free Software			|
|	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	|
|																				|
\*-----------------------------------------------------------------------------*/

// s1 special stage decompressor.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//void bitcheck(void);
bool getvalue(void);
unsigned char getbits(int number, unsigned char* input, int *offset);

namespace
{

char processed_bits = 0;
char packet_length = 0;
char mode = 0;
char output_repeatcount = 0;
//char remaining_bits = 0x10;
char bits_remaining_postfunc = 0;
char bitmask = 0;
unsigned short input_stream = 0;
unsigned short incrementing_value = 0;
unsigned short common_value = 0;
unsigned short addvalue = 0;
unsigned short outvalue = 0;
int loopcount;

FILE *infile;
FILE *outfile;
int infilelength;
int offset = 0;
unsigned char *pointer = 0;
unsigned char *outpointer = 0;
unsigned char *paddingpointer = 0;
int outoffset = 0;
bool done = false;
int result;

int remaining_bits = 8;
unsigned char input_buffer;
bool init = true;

}

/////////////////////////////////////////////////////////////////////////////////////////
//Program entry point
long EDecomp(char *SrcFile, char *DstFile, long Pointer, bool padding)
{
// The original code by Nemesis was command line based, so, in order to make as few changes as
// possible I simply copied the arguments passed to the function into the variable that is used
// to receive the command line arguments.
	char argv[4][260];
	strcpy(argv[1], SrcFile);
	strcpy(argv[2], DstFile);
	sprintf(argv[3], "0x%lX", Pointer);

// I prefered to initialize the following variables before starting the algorithm.
	processed_bits = 0;
	packet_length = 0;
	mode = 0;
	output_repeatcount = 0;
//	remaining_bits = 0x10;
	bits_remaining_postfunc = 0;
	bitmask = 0;
	input_stream = 0;
	incrementing_value = 0;
	common_value = 0;
	addvalue = 0;
	outvalue = 0;
	offset = 0;
	outoffset = 0;
	done = false;

	remaining_bits = 8;
	init = true;

// There starts the original code by Nemesis (all Console I/O operations were removed)
	infile = fopen(argv[1], "rb");
	if (infile == NULL)
	{
//		cout << "\n\nInvalid rom filename!";
		return -1;
	}
	fseek(infile, 0, SEEK_END);
	infilelength = ftell(infile);
	rewind(infile);
	pointer = (unsigned char *)calloc(infilelength, 0x01);
	outpointer = (unsigned char *)calloc(0x100000, 0x01);
	paddingpointer = (unsigned char *)calloc(0x1000, 0x01);

	fread(pointer, 1, infilelength, infile);

	if(argv[3][1] == 'x')
	{
		loopcount = 2;
	}
	else
	{
		loopcount = 0;
	}
	for(; argv[3][loopcount] != 0x00;loopcount++)
	{
		offset <<= 4;
		if(((argv[3][loopcount] & 0xF0) == 0x40) || ((argv[3][loopcount] & 0xF0) == 0x60))
		{
			offset |= ((argv[3][loopcount] & 0x0F) + 0x09);
		}
		else
		{
			offset |= (argv[3][loopcount] & 0x0F);
		}
	}
	int originaloffset = offset;
//	printf("Decompressing file....\t");


/////////////////////////////////////////////////////////////////////////////////////////
//Entry point
	packet_length = getbits(8, pointer, &offset);
	bitmask = getbits(8, pointer, &offset);
	incrementing_value = getbits(8, pointer, &offset) << 8;
	incrementing_value |= getbits(8, pointer, &offset);
	common_value = getbits(8, pointer, &offset) << 8;
	common_value |= getbits(8, pointer, &offset);

/////////////////////////////////////////////////////////////////////////////////////////
//Main algorithm
	for(;!done;)
	{
		if((getbits(1, pointer, &offset)) == 1)
		{
			mode = getbits(2, pointer, &offset);
			switch(mode)
			{
			case 0:
			case 1:
			case 2:
				{
					output_repeatcount = getbits(4, pointer, &offset);
					getvalue();
					for(;output_repeatcount >= 0; output_repeatcount--)
					{
						*(outpointer + outoffset++) = ((outvalue >> 8) & 0x00FF);
						*(outpointer + outoffset++) = (outvalue & 0x00FF);

						switch(mode)
						{
						case 0:
							break;
						case 1:
							outvalue++;
							break;
						case 2:
							outvalue--;
							break;
						}
					}
					break;
				}
			case 3:
				{
					output_repeatcount = getbits(4, pointer, &offset);
					if(output_repeatcount != 0x0F)
					{
						for(;output_repeatcount >= 0; output_repeatcount--)
						{
							getvalue();
							*(outpointer + outoffset++) = ((outvalue >> 8) & 0x00FF);
							*(outpointer + outoffset++) = (outvalue & 0x00FF);
						}
					}
					else
					{
						done = true;
					}
					break;
				}
			}
		}
		else
		{
			if((getbits(1, pointer, &offset)) == 0)
			{
				output_repeatcount = getbits(4, pointer, &offset);
				for(; output_repeatcount >= 0; output_repeatcount--)
				{
					*(outpointer + outoffset++) = ((incrementing_value >> 8) & 0x00FF);
					*(outpointer + outoffset++) = (incrementing_value & 0x00FF);
					incrementing_value++;
				}
			}
			else
			{
				output_repeatcount = getbits(4, pointer, &offset);
				for(; output_repeatcount >= 0; output_repeatcount--)
				{
					*(outpointer + outoffset++) = ((common_value >> 8) & 0x00FF);
					*(outpointer + outoffset++) = (common_value & 0x00FF);
				}
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////
//Output completed file
	outfile = fopen(argv[2], "wb");

	if(padding)
	{
		fwrite(paddingpointer, 1, 0x1000, outfile);
		for(loopcount = 0; loopcount < 0x2000; loopcount += 0x80)
		{
			fwrite(paddingpointer, 1, 0x20, outfile);
			fwrite(outpointer + (loopcount / 2) , 1, 0x40, outfile);
			fwrite(paddingpointer, 1, 0x20, outfile);
		}
		fwrite(paddingpointer, 1, 0x1000, outfile);
	}
	else
	{
		fwrite(outpointer, 1, outoffset, outfile);
	}

//	printf("Done!\n\n");
//	printf("Archive in %s successfully extracted to file %s.\n\n", argv[1], argv[2]);
//	printf("Compressed file size:\t\t0x%X\t\t%i\n", (offset - originaloffset), (offset - originaloffset));
//	printf("Uncompressed file size:\t\t0x%X\t\t%i\n", _tell(outfile), _tell(outfile));
//	printf("Location in file:\t\t0x%X - 0x%X\n", originaloffset, offset);
//	printf("Compression rate:\t\t%.2f%%\n\n", (100 - (((float)(offset - originaloffset) / (float)(_tell(outfile))) * 100)));

	free(pointer);
	free(outpointer);
	free(paddingpointer);
	fclose(outfile);
	fclose(infile);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//Getvalue function		- This function extracts a 16-bit value from the compressed data.
bool getvalue(void)
{
	addvalue = 0;
	for(loopcount = 0; loopcount < 5; loopcount++)
	{
		if(((bitmask >> (4 - loopcount)) & 0x01) != 0)
		{
			addvalue |= (getbits(1, pointer, &offset) << (0xF - loopcount));
		}
	}

	if(packet_length > 8)
	{
		outvalue = getbits(packet_length - 8, pointer, &offset) << 8;
		outvalue |= getbits(8, pointer, &offset);
	}
	else
	{
		outvalue = getbits(packet_length, pointer, &offset);
	}

	outvalue &= (0xFFFF ^ (0xFFFF << packet_length));
	outvalue += addvalue;
	return true;
}


unsigned char getbits(int number, unsigned char* input, int *offset)
{
	if (init) { input_buffer = *(input + (*offset)++); init = false; }
	unsigned char value = 0;

	if(number > remaining_bits)
	{
		value = input_buffer >> (8 - number);
		input_buffer = *(input + (*offset)++);
		value |= ((input_buffer & (0xFF << (8 - number + remaining_bits))) >> (8 - number + remaining_bits));
		input_buffer <<= (number - remaining_bits);
		remaining_bits = 8 - (number - remaining_bits);
	}
	else
	{
		value = ((input_buffer & (0xFF << (8 - number))) >> (8 - number));
		remaining_bits -= number;
		input_buffer <<= number;
	}

	return value;
}
