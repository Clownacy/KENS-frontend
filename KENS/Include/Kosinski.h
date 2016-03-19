/*-----------------------------------------------------------------------------*\
|																				|
|	Kosinski.dll: Compression / Decompression of data in Kosinski format		|
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

#ifndef _KOSINSKI_H_
#define _KOSINSKI_H_

#include <windows.h>

long (__cdecl *KComp)(char *SrcFile, char *DstFile, bool Moduled);
long (__cdecl *KDecomp)(char *SrcFile, char *DstFile, long Pointer, bool Moduled);
long (__cdecl *KCompEx)(char *SrcFile, char *DstFile, long SlideWin, long RecLen, bool Moduled);
long (__cdecl *KCompToBuf)(char *SrcFile, char *&DstBuffer, long *BufSize, bool Moduled);
long (__cdecl *KDecompToBuf)(char *SrcFile, char *&DstBuffer, long *BufSize, long Pointer, bool Moduled);
long (__cdecl *KCompToBufEx)(char *SrcFile, char *&DstBuffer, long *BufSize, long SlideWin, long RecLen, bool Moduled);
long (__cdecl *KFreeBuffer)(char *Buffer);

bool KInit(const char* const DLL)
{
	HMODULE hKosinski = LoadLibrary(DLL);
	if (!hKosinski) return false;

	KComp = (long (*)(char *, char *, bool))GetProcAddress(hKosinski, "Comp");
	KDecomp = (long (*)(char *, char *, long, bool))GetProcAddress(hKosinski, "Decomp");
	KCompEx = (long (*)(char *, char *, long, long, bool))GetProcAddress(hKosinski, "CompEx");
	KCompToBuf = (long (*)(char *, char *&, long *, bool))GetProcAddress(hKosinski, "CompToBuf");
	KDecompToBuf = (long (*)(char *, char *&, long *, long, bool))GetProcAddress(hKosinski, "DecompToBuf");
	KCompToBufEx = (long (*)(char *, char *&, long *, long, long, bool))GetProcAddress(hKosinski, "CompToBufEx");
	KFreeBuffer = (long (*)(char *))GetProcAddress(hKosinski, "FreeBuffer");

	if (KComp==NULL) return false;
	if (KDecomp==NULL) return false;
	if (KCompEx==NULL) return false;
	if (KCompToBuf==NULL) return false;
	if (KDecompToBuf==NULL) return false;
	if (KCompToBufEx==NULL) return false;
	if (KFreeBuffer==NULL) return false;

	return true;
}


#endif /* _KOSINSKI_H_ */
