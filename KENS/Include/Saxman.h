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

#ifndef _SAXMAN_H_
#define _SAXMAN_H_

#include <windows.h>

long (__cdecl *SComp)(char *SrcFile, char *DstFile, bool WithSize);
long (__cdecl *SDecomp)(char *SrcFile, char *DstFile, long Pointer, unsigned short Size);
long (__cdecl *SCompToBuf)(char *SrcFile, char *&DstBuffer, long *BufSize, bool WithSize);
long (__cdecl *SDecompToBuf)(char *SrcFile, char *&DstBuffer, long *BufSize, long Pointer, unsigned short Size);
long (__cdecl *SFreeBuffer)(char *Buffer);

bool SInit(const char* const DLL)
{
	HMODULE hSaxman = LoadLibrary(DLL);
	if (!hSaxman) return false;

	SComp = (long (*)(char *, char *, bool))GetProcAddress(hSaxman, "Comp2");
	SDecomp = (long (*)(char *, char *, long, unsigned short))GetProcAddress(hSaxman, "Decomp2");
	SCompToBuf = (long (*)(char *, char *&, long *, bool))GetProcAddress(hSaxman, "CompToBuf2");
	SDecompToBuf = (long (*)(char *, char *&, long *, long, unsigned short))GetProcAddress(hSaxman, "DecompToBuf2");
	SFreeBuffer = (long (*)(char *))GetProcAddress(hSaxman, "FreeBuffer");

	if (SComp==NULL) return false;
	if (SDecomp==NULL) return false;
	if (SCompToBuf==NULL) return false;
	if (SDecompToBuf==NULL) return false;
	if (SFreeBuffer==NULL) return false;

	return true;
}


#endif /* _SAXMAN_H_ */