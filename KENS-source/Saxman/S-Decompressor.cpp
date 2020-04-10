/*-----------------------------------------------------------------------------*\
|																				|
|	Saxman.dll: Compression / Decompression of data in Saxman format			|
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

#include <stdio.h>

extern unsigned long GetFileSize(FILE *File);

#define SUCCESS										0x00
#define ERROR_UNKNOWN								0x01
#define ERROR_SOURCE_FILE_DOES_NOT_EXIST			0x02

//-----------------------------------------------------------------------------------------------
// Name: SDecomp(char *SrcFile, char *DstFile, long Location, unsigned short Size)
// Desc: Decompresses the data using the Saxman compression format
//-----------------------------------------------------------------------------------------------
long SDecomp(char *SrcFile, char *DstFile, long Location, unsigned short Size)
{
// Files
	FILE *Src;
	FILE *Dst;

// Info Byte, Flag, Count and Offset (with initial values)
	unsigned char InfoByte = 0;
	unsigned char IBP = 8;
	unsigned char Flag = 0;
	unsigned char Count = 0;
	unsigned short Offset = 0;
//	unsigned short Size = 0;		// Size of the compressed data

// Other info
	unsigned char Byte;				// Used to store a Byte temporarly
	int Pointer;					// Used to store a Pointer temporarly
	int i;							// Counter

//------------------------------------------------------------------------------------------------

	Src=fopen(SrcFile,"rb");
	if (Src==NULL) return ERROR_SOURCE_FILE_DOES_NOT_EXIST;
	Dst=fopen(DstFile,"w+b");

	fseek(Src, Location, SEEK_SET);

	if (!Size)
	{
		fread(&Size, 2, 1, Src);
		Size += 2;
	}

	while(1)
	{
		if (IBP==8) { IBP=0; if (fread(&InfoByte, 1, 1, Src)==0) break; if (ftell(Src) >= Location + Size) break; }
		Flag = ( InfoByte >> (IBP++) ) & 1;
		switch(Flag)
		{
			case 0:
				Offset=0; // See 3 lines below
				if (fread(&Offset, 1, 1, Src)==0) break; if (ftell(Src) >= Location + Size) break;
				if (fread(&Count, 1, 1, Src)==0) break; if (ftell(Src) >= Location + Size) break;
				Offset = ( Offset | ((Count & 0xF0) << 4) ) + 0x12; // Can be improved
				Offset |= (ftell(Dst) & 0xF000);
				Count&=0x0F;
				Count+=3;
				if(Offset>=ftell(Dst))
				{
					Offset -= 0x1000;
				}
				if (Offset<ftell(Dst))
				{
					for (i=0; i<Count; ++i)
					{
						Pointer=ftell(Dst);
						fseek(Dst, Offset + i, SEEK_SET);
						if (fread(&Byte, 1, 1, Dst)==0) break;
						fseek(Dst, Pointer, SEEK_SET);
						fwrite(&Byte, 1, 1, Dst);
					}
				}
				else
				{
					Byte=0;
					for (i=0; i<Count; ++i)
					{
						fwrite(&Byte, 1, 1, Dst);
					}
				}
				break;

			case 1:
				if (fread(&Byte, 1, 1, Src)==0) break; if (ftell(Src) >= Location + Size) break;
				fwrite(&Byte, 1, 1, Dst);
				break;
		}
	}

//------------------------------------------------------------------------------------------------

	fclose(Dst);
	fclose(Src);
	return SUCCESS;
}
