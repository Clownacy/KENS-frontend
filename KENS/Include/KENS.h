/*-----------------------------------------------------------------------------*\
|																				|
|	Kosinski.dll, Enigma.dll, Nemesis.dll, Saxman.dll: Compression and			|
|	Decompression of data in Kosinski, Enimga, Nemesis and Saxman formats		|
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

#ifndef _KENS_H_
#define _KENS_H_

#include <windows.h>

long (__cdecl *KComp)(char *SrcFile, char *DstFile, bool Moduled);
long (__cdecl *KDecomp)(char *SrcFile, char *DstFile, long Pointer, bool Moduled);
long (__cdecl *KCompEx)(char *SrcFile, char *DstFile, long SlideWin, long RecLen, bool Moduled);
long (__cdecl *KCompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize, bool Moduled);
long (__cdecl *KDecompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize, long Pointer, bool Moduled);
long (__cdecl *KCompToBufEx)(char *SrcFile, char *DstBuffer, long *BufSize, long SlideWin, long RecLen, bool Moduled);
long (__cdecl *KFreeBuffer)(char *Buffer);

long (__cdecl *EComp)(char *SrcFile, char *DstFile, bool Padding);
long (__cdecl *EDecomp)(char *SrcFile, char *DstFile, long Pointer, bool Padding);
long (__cdecl *ECompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize, bool Padding);
long (__cdecl *EDecompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize, long Pointer, bool Padding);
long (__cdecl *EFreeBuffer)(char *Buffer);

long (__cdecl *NComp)(char *SrcFile, char *DstFile);
long (__cdecl *NDecomp)(char *SrcFile, char *DstFile, long Pointer);
long (__cdecl *NCompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize);
long (__cdecl *NDecompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize, long Pointer);
long (__cdecl *NFreeBuffer)(char *Buffer);

long (__cdecl *SComp)(char *SrcFile, char *DstFile, bool WithSize);
long (__cdecl *SDecomp)(char *SrcFile, char *DstFile, long Pointer, unsigned short Size);
long (__cdecl *SCompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize, bool WithSize);
long (__cdecl *SDecompToBuf)(char *SrcFile, char *DstBuffer, long *BufSize, long Pointer, unsigned short Size);
long (__cdecl *SFreeBuffer)(char *Buffer);

bool KInit(const char* const DLL)
{
	HMODULE hKosinski = LoadLibrary(DLL);
	if (!hKosinski) return false;

	KComp = (long (*)(char *, char *, bool))(void(*)(void))GetProcAddress(hKosinski, "Comp");
	KDecomp = (long (*)(char *, char *, long, bool))(void(*)(void))GetProcAddress(hKosinski, "Decomp");
	KCompEx = (long (*)(char *, char *, long, long, bool))(void(*)(void))GetProcAddress(hKosinski, "CompEx");
	KCompToBuf = (long (*)(char *, char *, long *, bool))(void(*)(void))GetProcAddress(hKosinski, "CompToBuf");
	KDecompToBuf = (long (*)(char *, char *, long *, long, bool))(void(*)(void))GetProcAddress(hKosinski, "DecompToBuf");
	KCompToBufEx = (long (*)(char *, char *, long *, long, long, bool))(void(*)(void))GetProcAddress(hKosinski, "CompToBufEx");
	KFreeBuffer = (long (*)(char *))(void(*)(void))GetProcAddress(hKosinski, "FreeBuffer");

	if (KComp==NULL) return false;
	if (KDecomp==NULL) return false;
	if (KCompEx==NULL) return false;
	if (KCompToBuf==NULL) return false;
	if (KDecompToBuf==NULL) return false;
	if (KCompToBufEx==NULL) return false;
	if (KFreeBuffer==NULL) return false;

	return true;
}

bool EInit(const char* const DLL)
{
	HMODULE hEnigma = LoadLibrary(DLL);
	if (!hEnigma) return false;

	EComp = (long (*)(char *, char *, bool))(void(*)(void))GetProcAddress(hEnigma, "Comp");
	EDecomp = (long (*)(char *, char *, long, bool))(void(*)(void))GetProcAddress(hEnigma, "Decomp");
	ECompToBuf = (long (*)(char *, char *, long *, bool))(void(*)(void))GetProcAddress(hEnigma, "CompToBuf");
	EDecompToBuf = (long (*)(char *, char *, long *, long, bool))(void(*)(void))GetProcAddress(hEnigma, "DecompToBuf");
	EFreeBuffer = (long (*)(char *))(void(*)(void))GetProcAddress(hEnigma, "FreeBuffer");

	if (EComp==NULL) return false;
	if (EDecomp==NULL) return false;
	if (ECompToBuf==NULL) return false;
	if (EDecompToBuf==NULL) return false;
	if (EFreeBuffer==NULL) return false;

	return true;
}

bool NInit(const char* const DLL)
{
	HMODULE hNemesis = LoadLibrary(DLL);
	if (!hNemesis) return false;

	NComp = (long (*)(char *, char *))(void(*)(void))GetProcAddress(hNemesis, "Comp");
	NDecomp = (long (*)(char *, char *, long))(void(*)(void))GetProcAddress(hNemesis, "Decomp");
	NCompToBuf = (long (*)(char *, char *, long *))(void(*)(void))GetProcAddress(hNemesis, "CompToBuf");
	NDecompToBuf = (long (*)(char *, char *, long *, long))(void(*)(void))GetProcAddress(hNemesis, "DecompToBuf");
	NFreeBuffer = (long (*)(char *))(void(*)(void))GetProcAddress(hNemesis, "FreeBuffer");

	if (NComp==NULL) return false;
	if (NDecomp==NULL) return false;
	if (NCompToBuf==NULL) return false;
	if (NDecompToBuf==NULL) return false;
	if (NFreeBuffer==NULL) return false;

	return true;
}

bool SInit(const char* const DLL)
{
	HMODULE hSaxman = LoadLibrary(DLL);
	if (!hSaxman) return false;

	SComp = (long (*)(char *, char *, bool))(void(*)(void))GetProcAddress(hSaxman, "Comp2");
	SDecomp = (long (*)(char *, char *, long, unsigned short))(void(*)(void))GetProcAddress(hSaxman, "Decomp2");
	SCompToBuf = (long (*)(char *, char *, long *, bool))(void(*)(void))GetProcAddress(hSaxman, "CompToBuf2");
	SDecompToBuf = (long (*)(char *, char *, long *, long, unsigned short))(void(*)(void))GetProcAddress(hSaxman, "DecompToBuf2");
	SFreeBuffer = (long (*)(char *))(void(*)(void))GetProcAddress(hSaxman, "FreeBuffer");

	if (SComp==NULL) return false;
	if (SDecomp==NULL) return false;
	if (SCompToBuf==NULL) return false;
	if (SDecompToBuf==NULL) return false;
	if (SFreeBuffer==NULL) return false;

	return true;
}


#endif /* _KENS_H_ */
