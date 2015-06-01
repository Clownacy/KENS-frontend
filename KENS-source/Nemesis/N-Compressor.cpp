/*-----------------------------------------------------------------------------*\
|																				|
|	Nemesis.dll: Compression / Decompression of data in Nemesis format			|
|	Copyright © 2002-2004 The KENS Project Development Team						|
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

/*----------------------------Nemesis compressor MKII-----------------------------------------------------------*\
| Function:
|	Compresses a set of 8x8 tiles for the Genesis into the Nemesis compression format.
|
| Operation:
|	NEMCMP2 infile outfile [offset]
|	
|	infile:		The file that contains the uncompressed art.
|	outfile:	The name of the file to create containing the compressed art.
|	[offset]:	The offset in the file to start compressing from in base 16.
|
|
|											Version 1.1.1	(public release)
|											Programmed by Roger Sanders (AKA Nemesis)
|											Created 17/8/2003  Last modified 28/8/2003
\*-------------------------------------------------------------------------------------*/

#include "iostream.h"
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compress(int mode);
int getnybble(void);
unsigned char writedata(int data, int writecount);
short generate_entry(unsigned char packet_length, unsigned char repeatcount, unsigned char nybble);
int checknybble(int offset);
void flush_writebuffer(void);


/**********************************Variable block***************************************\
|	Contains the definitions for the global variables used in the program
\**********************************Variable block***************************************/
int loopcount2 = 0;
int tempoffset = 0;
int tiles = 0;
int bitoffset = 4;
int loopcount;
int infile;
int outfile;
int infilelength;
int offset = 0;
int waitingbits = 0;
unsigned char bytebuffer = 0;
unsigned char *infilepointer = 0;
unsigned char *workingpointer = 0;
unsigned char *mode0cmppointer = 0;
unsigned char *mode1cmppointer = 0;
int workingoffset = 0;
int mode0cmpoffset = 0;
int mode1cmpoffset = 0;
int outoffset = 0;
bool padding = false;
bool workaround;
bool extended = false;
bool othervalues = false;
int result;
short readbuffer = 0;
short checkbuffer = 0;
unsigned int occurance_table[0x10][0x08] = {0x00};
unsigned int linear_array[0x80] = {0x00};
unsigned short repetition_table[0x100] = {0x00};
unsigned char currentnybble = 0;

/***********************************Entry point*****************************************\
|
\***********************************Entry point*****************************************/
long __cdecl NComp(char *SrcFile, char *DstFile)
{
// The original code by Nemesis allowed to pass a pointer to the location of the source
// file to start compressing from, but I'm not using it, for simplicity
	long Pointer = 0;

// The original code by Nemesis was command line based, so, in order to make as few changes as
// possible I simply copied the arguments passed to the function into the variable that is used
// to receive the command line arguments
	char argv[4][260];
	strcpy(argv[1], SrcFile);
	strcpy(argv[2], DstFile);
	strcpy(argv[3], "0x");
	itoa(Pointer, argv[3]+2, 16);

// I prefered to initialize the following variables before starting the algorithm.
	loopcount2 = 0;
	tempoffset = 0;
	tiles = 0;
	bitoffset = 4;
	offset = 0;
	waitingbits = 0;
	bytebuffer = 0;
	infilepointer = 0;
	workingpointer = 0;
	mode0cmppointer = 0;
	mode1cmppointer = 0;
	workingoffset = 0;
	mode0cmpoffset = 0;
	mode1cmpoffset = 0;
	outoffset = 0;
	padding = false;
	extended = false;
	othervalues = false;
	readbuffer = 0;
	checkbuffer = 0;
	memset(&occurance_table, 0, 0x10 * 0x08);
	memset(&linear_array, 0, 0x80);
	memset(&repetition_table, 0, 0x100);
	currentnybble = 0;

// There starts the original code by Nemesis (all Console I/O opperations were removed)
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

/////////////////////////////////////////////////////////////////////////////////////////
//File handling
	infile = _open(argv[1], _S_IWRITE, _O_BINARY);
	if (infile < 0)
	{
//		cout << "\n\nError opening source file!";
		return -1;
	}
	outfile = _creat(argv[2], _S_IWRITE);
	result = _setmode(infile, _O_BINARY);
	result = _setmode(outfile, _O_BINARY);
	infilelength = _filelength(infile);
	if(infilelength < offset)
	{
		printf("\n\nInvalid offset!!");
	}
	infilepointer = (unsigned char *)calloc((infilelength - offset), 0x01);
	mode0cmppointer = (unsigned char *)calloc((infilelength - offset), 0x01);
	mode1cmppointer = (unsigned char *)calloc((infilelength - offset), 0x01);
	_lseek(infile, offset, SEEK_SET);
	_read(infile, infilepointer, (infilelength - offset));

//	printf("\n\nBeginning mode 0 compression\n");
	workingpointer = mode0cmppointer;
	compress(0);
	mode0cmpoffset = workingoffset;
//	printf("\n\nBeginning mode 1 compression\n");
	workingoffset = 0;
	tempoffset = 0;
	waitingbits = 0;
	bitoffset = 4;
	workingpointer = mode1cmppointer;
	for(loopcount = ((infilelength - offset) - 4); loopcount > 0; loopcount -= 4)
	{
		*(infilepointer + loopcount) ^= *(infilepointer + loopcount - 4);
		*(infilepointer + loopcount + 1) ^= *(infilepointer + loopcount - 3);
		*(infilepointer + loopcount + 2) ^= *(infilepointer + loopcount - 2);
		*(infilepointer + loopcount + 3) ^= *(infilepointer + loopcount - 1);
	}
	compress(1);
	mode1cmpoffset = workingoffset;

	if(mode1cmpoffset < mode0cmpoffset)
	{
		_write(outfile, mode1cmppointer, mode1cmpoffset);
//		printf("\n\n\n%s compressed into %s from offset %i using mode 1\nOriginal filesize:\t%i\nCompressed filesize:\t%i\nPercentage of original:\t%f", argv[1], argv[2], offset, infilelength, mode1cmpoffset, (((float)mode1cmpoffset / (float)infilelength) * 100));
	}
	else
	{
		_write(outfile, mode0cmppointer, mode0cmpoffset);
//		printf("\n\n\n%s compressed into %s from offset %i using mode 0\nOriginal filesize:\t%i\nCompressed filesize:\t%i\nPercentage of original:\t%f", argv[1], argv[2], offset, infilelength, mode0cmpoffset, (((float)mode0cmpoffset / (float)infilelength) * 100));
	}

	if (_tell(outfile) & 1) _lseek(outfile, 1, SEEK_CUR);

/////////////////////////////////////////////////////////////////////////////////////////
//Cleanup
	free(infilepointer);
	free(mode0cmppointer);
	free(mode1cmppointer);
	_close(infile);
	_close(outfile);

	return 0;
}

int compress(int mode)
{
//	printf("Profiling data....\t\t\t\t");
/////////////////////////////////////////////////////////////////////////////////////////
//Load value/repeatcount template into occurance table and linear array
	tiles = ((infilelength - offset) / 0x20);
	int currentval = getnybble();
	int tempval = 0;
	int hitcount = 0;
	for(loopcount = 0; loopcount < 0x10; loopcount++)
	{
		for(loopcount2 = 0; loopcount2 < 0x08; loopcount2++)
		{
			occurance_table[loopcount][loopcount2] = ((loopcount << 0x18) | (loopcount2 << 0x10));
			linear_array[(loopcount2 << 4) | loopcount] = ((loopcount << 0x18) | (loopcount2 << 0x10));
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////
//Calculate occurance values
	for(loopcount = 2; loopcount < ((infilelength - offset) * 2); loopcount++)
	{
		tempval = getnybble();
		if((tempval != currentval) || (hitcount >= 7))
		{
			occurance_table[currentval][hitcount]++;
			linear_array[(hitcount << 4) | currentval] = occurance_table[currentval][hitcount];
			currentval = tempval;
			hitcount = 0;
		}
		else
		{
			hitcount++;
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////
//Sort linear array
	tempoffset = 0;
	int tempval1 = 0;
	for(loopcount2 = 0; loopcount2 < 0x80; loopcount2++)
	{
		for(loopcount = 0; loopcount < 0x7F; loopcount++)
		{
			if(((linear_array[loopcount + 1]) & 0xFFFF) > ((linear_array[loopcount]) & 0xFFFF))
			{
				tempval1 = linear_array[loopcount];
				linear_array[loopcount] = linear_array[loopcount + 1];
				linear_array[loopcount + 1] = tempval1;
			}
		}
	}
//	printf("Complete\n");

/////////////////////////////////////////////////////////////////////////////////////////
//Write header
	writedata(mode, 1);
	writedata(tiles, 15);

/////////////////////////////////////////////////////////////////////////////////////////
//Generate repetition buffer
//	printf("Generating repetition dictionary....\t\t");
	int packet_length = 0x07;
	int loopcount3 = 0;
	int increase_array[0x100];

//Generate the base repetition table from the linear array
	for(loopcount = 0; loopcount < 0x100; loopcount++)
	{
		repetition_table[loopcount] = 0;
	}
	for(loopcount = 0; (loopcount < 0x80) && ((linear_array[loopcount] & 0xFFFF) > 0); loopcount++)
	{
		repetition_table[loopcount] = generate_entry(8, ((linear_array[loopcount] & 0x00FF0000) >> 0x10), ((linear_array[loopcount] & 0xFF000000) >> 0x18));
	}

/////////////////////////////////////////////////////////////////////////////////////////
//Begin repetition table optimisation loop
	for(;;)
	{
//Wipe the increase array
		for(loopcount = 0; loopcount < 0x100; loopcount++)
		{
			increase_array[loopcount] = 0;
		}
//Calculate increase values
		for(loopcount = 0; (loopcount < 0xFC) && (repetition_table[loopcount] & 0x0F00) > 0; loopcount++)
		{
			if(((repetition_table[loopcount] & 0xFF00) > 0x100) && ((((1 << (8 - ((repetition_table[loopcount] & 0x0F00) >> 8))) * 2) + loopcount) < 0xFC))
			{
				increase_array[loopcount] = (occurance_table[repetition_table[loopcount] & 0x000F][(repetition_table[loopcount] & 0x00F0) >> 4] & 0xFFFF);
				increase_array[loopcount] |= (repetition_table[loopcount] & 0x000F) << 24;
				increase_array[loopcount] |= ((repetition_table[loopcount] & 0x00F0) >> 4) << 16;

				int bufferedval = 0;
				for(loopcount2 = (0xFC - (1 << (8 - ((int)(repetition_table[loopcount] & 0x0F00) >> 8)))); loopcount2 < 0xFC; loopcount2++)
				{
					othervalues = false;
					for(loopcount3 = 0; loopcount3 < 0xFC; loopcount3++)
					{
						if(((repetition_table[loopcount3] & 0x000F) == (repetition_table[loopcount2] & 0x000F)) && ((repetition_table[loopcount2] & 0x0F00) > 0)&& ((repetition_table[loopcount3] & 0x0F00) > 0))
						{
							othervalues = true;
						}
					}
					if(((repetition_table[loopcount2] & 0x0F00) > 0) & (bufferedval != repetition_table[loopcount2]))
					{
						int addvalue = 0;
						if(othervalues == true)
						{
							addvalue = -16;
						}
						else
						{
							addvalue = -24;
						}
						increase_array[loopcount] = (((increase_array[loopcount] - (addvalue + ((occurance_table[repetition_table[loopcount2] & 0x000F][(repetition_table[loopcount2] & 0x00F0) >> 4] & 0xFFFF) * (5 + (8 - ((repetition_table[loopcount2] & 0x0F00) >> 8)))))) & 0x0000FFFF) | (increase_array[loopcount] & 0xFFFF0000));
						bufferedval = repetition_table[loopcount2];
					}
				}
				if((increase_array[loopcount] & 0x0000FFFF) < 0x8000)
				{
					increase_array[loopcount] = (increase_array[loopcount] & 0xFFFF0000) | ((int)((float)((float)1 / (float)(1 << (8 - ((repetition_table[loopcount] & 0x0F00) >> 8)))) * (occurance_table[(repetition_table[loopcount] & 0x000F)][((repetition_table[loopcount] & 0x00F0) >> 4)] & 0x0000FFFF)) & 0x0000FFFF);
				}
			}
		}

//sort increase array
		for(loopcount2 = 0; loopcount2 < 0xFC; loopcount2++)
		{
			for(loopcount = 0; loopcount < 0xFC; loopcount++)
			{
				if(((increase_array[loopcount + 1] & 0x8000) != 0) | ((increase_array[loopcount] & 0x8000) != 0))
				{
					if((increase_array[loopcount] & 0xFFFF) > (increase_array[loopcount + 1] & 0xFFFF))
					{
						tempval1 = increase_array[loopcount];
						increase_array[loopcount] = increase_array[loopcount + 1];
						increase_array[loopcount + 1] = tempval1;
					}
				}
				else
				{
					if((increase_array[loopcount + 1] & 0xFFFF) > (increase_array[loopcount] & 0xFFFF))
					{
						tempval1 = increase_array[loopcount];
						increase_array[loopcount] = increase_array[loopcount + 1];
						increase_array[loopcount + 1] = tempval1;
					}
				}
			}
		}

//If no improvements can be made, exit
		if(((increase_array[0] & 0x00008000) > 0) | ((increase_array[0] & 0x0000FFFF) == 0))
		{
			break;
		}

//Find entry to be modified and reduce bitcount
		bool finddone = false;
		for(loopcount2 = 0; (!finddone) && (loopcount2 < 0xFC); loopcount2++)
		{
			for(loopcount = 0; (((((increase_array[loopcount2] & 0xFF000000) >> 24) | ((increase_array[loopcount2] & 0x00FF0000) >> 12)) ^ (repetition_table[loopcount] & 0x00FF)) != 0) & (loopcount < 0xFC); loopcount++)
			{
			}
			if(loopcount < 0xFC)
			{
				repetition_table[loopcount] -= 0x100;
				finddone = true;
			}
			else
			{
				break;
			}
		}

//Shift values in array along
		for(loopcount2 = (0xFB - (1 << (8 - ((repetition_table[loopcount] >> 8) + 1)))); loopcount2 >= loopcount; loopcount2--)
		{
			repetition_table[loopcount2 + (1 << (8 - ((repetition_table[loopcount] >> 8) + 1)))] = repetition_table[loopcount2];
		}

//Lengthen modified entry
		for(loopcount2 = loopcount; loopcount2 < ((1 << (8 - (repetition_table[loopcount] >> 8))) + loopcount); loopcount2++)
		{
			repetition_table[loopcount2] = repetition_table[loopcount];
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////
//Load index values of entries in repetition table back into occurance table
	int tempstore = 0xFFFFFFFF;
	for(loopcount = 0; loopcount < 0xFC; loopcount++)
	{
		if((tempstore != repetition_table[loopcount]) && ((repetition_table[loopcount] & 0x0F00) != 0))
		{
			occurance_table[repetition_table[loopcount] & 0x000F][(repetition_table[loopcount] & 0x00F0) >> 4] = (0x80000000 | loopcount) | ((repetition_table[loopcount] >> 8) << 0x14);
		}
		tempstore = repetition_table[loopcount];
	}
//	printf("Complete\n");

/////////////////////////////////////////////////////////////////////////////////////////
//First stage compression
//	printf("Compressing repetition dictionary...\t\t");
	int writtenvalues = 0;
	bool newval = true;
	for(loopcount = 0x00; loopcount < 0x10; loopcount++)
	{
		newval = true;
		for(loopcount2 = 0; (loopcount2 < 0x100) && (writtenvalues < 0x100);)
		{
			for(; (((repetition_table[loopcount2] & 0x000F) != loopcount) || ((repetition_table[loopcount2] & 0x0F00) == 0)) && (loopcount2 < 0x100); loopcount2++)
			{
			}
			if(loopcount2 < 0x100)
			{
				if(newval)
				{
					writedata(8, 4);
					writedata(loopcount, 4);
					newval = false;
				}
				packet_length = ((repetition_table[loopcount2] >> 8) & 0x000F);
				writedata(repetition_table[loopcount2] >> 4, 4);
				writedata(packet_length, 4);
				writtenvalues += (1 << (8 - packet_length));
				writedata(loopcount2 >> (8 - packet_length), 8);
				loopcount2 += (1 << (8 - packet_length));
			}
		}
	}
	flush_writebuffer();
	writedata(0xFF, 8);
	printf("Complete\n");

/////////////////////////////////////////////////////////////////////////////////////////
//Second stage compression
//	printf("Compressing file....\t\t\t\t");
	for(tempoffset = 0; tempoffset < (infilelength - offset) ;)
	{
		currentnybble = getnybble();
		for(loopcount = 0; (checknybble(loopcount) == currentnybble) && (loopcount < 0x07) && ((tempoffset + (loopcount / 2)) < (infilelength - offset)); loopcount++)
		{
		}
		if(((occurance_table[currentnybble][loopcount]) & 0x80000000) > 0)
		{
			int temp166 = (((occurance_table[currentnybble][loopcount] >> 0x14) & 0x0000000F) - 1);
			for(loopcount2 = 7; (loopcount2 >= 0) && ((temp166 - (7 - loopcount2)) >= 0); loopcount2--)
			{
				writedata(((occurance_table[currentnybble][loopcount]) >> loopcount2) & 0x00000001, 1);
			}
			tempoffset += (loopcount >> 1);
			if((loopcount & 0x01) > 0)
			{
				if(bitoffset > 0)
				{
					bitoffset = 0;
					tempoffset++;
				}
				else
				{
					bitoffset = 4;
				}
			}
		}
		else
		{
			writedata(0x3F, 6);
			writedata(loopcount, 3);
			writedata(currentnybble, 4);
			tempoffset += (loopcount >> 1);
			if((loopcount & 0x01) > 0)
			{
				if(bitoffset > 0)
				{
					bitoffset = 0;
					tempoffset++;
				}
				else
				{
					bitoffset = 4;
				}
			}
		}
	}
	flush_writebuffer();
//	printf("Complete\n");
//	printf("Compressed filesize:\t\t\t\t%i", workingoffset);

	return 0;
}


/*******************************Getnybble function**************************************\
|	Returns the next nybble in the file and increments the read offset.
\*******************************Getnybble function**************************************/
int getnybble(void)
{
	if(bitoffset > 0)
	{
		bitoffset = 0;
		return (*(infilepointer + tempoffset++) & 0x0F);
	}
	else
	{
		bitoffset = 4;
		return ((*(infilepointer + tempoffset) & 0xF0) >> 4);
	}
}


/*******************************Checknybble function************************************\
|	Returns the nybble at the current offset location, plus the supplied offset in
|	nybbles, without incrementing the offset location.
\*******************************Checknybble function************************************/
int checknybble(int offset)
{
	if(bitoffset > 0)
	{
		offset += 1;
	}
	if((offset & 0x01) == 0x00)
	{
		return ((*(infilepointer + (offset / 2) + tempoffset) & 0xF0) >> 4);
	}
	else
	{
		return (*(infilepointer + (offset / 2) + tempoffset) & 0x0F);
	}
}


/*********************************Read/Write functions**********************************\
|	Adds the supplied bit to the write stream.
\*********************************Read/Write functions**********************************/
unsigned char writedata(int data, int writecount)
{
	for(int loopcount = 0; loopcount < writecount; loopcount++)
	{
		bytebuffer <<= 1;
		bytebuffer |= (data >> ((writecount - 1) - loopcount));
		waitingbits++;
		if(waitingbits >= 8)
		{
//			_write(outfile, &bytebuffer, 0x1);
			*(workingpointer + workingoffset++) = bytebuffer;
			waitingbits = 0;
		}
	}
	return 1;
}


/******************************Flush_writebuffer function*******************************\
|	If an incomplete byte is currently waiting in the output sream, this function
|	completes the packet with 0's and outputs it to the file.
\******************************Flush_writebuffer function*******************************/
void flush_writebuffer(void)
{
	if(waitingbits > 0)
	{
		bytebuffer <<= (8 - waitingbits);
//		_write(outfile, &bytebuffer, 0x1);
		*(workingpointer + workingoffset++) = bytebuffer;
		waitingbits = 0;
	}
}


/******************************Generate_entry function**********************************\
|	Generates a buffer entry value from the supplied parameters.
\******************************Generate_entry function**********************************/
short generate_entry(unsigned char packet_length, unsigned char repeatcount, unsigned char nybble)
{
	short bufferentry = 0;
	bufferentry |= ((packet_length & 0x0F) << 8);
	bufferentry |= ((repeatcount & 0x0F) << 4);
	bufferentry |= (nybble & 0x0F);
	return bufferentry;
}
