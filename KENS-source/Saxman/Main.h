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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

//-- Functions to Export --------------------------------------------------------
// Version 2 functions
extern "C" DLL_API long __cdecl Comp2(char *SrcFile, char *DstFile, bool WithSize);
extern "C" DLL_API long __cdecl Decomp2(char *SrcFile, char *DstFile, long Pointer, unsigned short Size);
extern "C" DLL_API long __cdecl CompToBuf2(char *SrcFile, char *&DstBuffer, long *BufSize, bool WithSize);
extern "C" DLL_API long __cdecl DecompToBuf2(char *SrcFile, char *&DstBuffer, long *BufSize, long Pointer, unsigned short Size);
extern "C" DLL_API long __cdecl FreeBuffer(char *&Buffer);
extern "C" DLL_API long __stdcall VBComp2(VARIANT SrcFile, VARIANT DstFile, bool WithSize);
extern "C" DLL_API long __stdcall VBDecomp2(VARIANT SrcFile, VARIANT DstFile, long Pointer, long Size);
extern "C" DLL_API double __cdecl GMComp2(char *SrcFile, char *DstFile, double WithSize);
extern "C" DLL_API double __cdecl GMDecomp2(char *SrcFile, char *DstFile, double Pointer, double Size);
// Version 1 functions
extern "C" DLL_API long __cdecl Comp(char *SrcFile, char *DstFile);
extern "C" DLL_API long __cdecl Decomp(char *SrcFile, char *DstFile, long Pointer);
extern "C" DLL_API long __stdcall VBComp(VARIANT SrcFile, VARIANT DstFile);
extern "C" DLL_API long __stdcall VBDecomp(VARIANT SrcFile, VARIANT DstFile, long Pointer);
extern "C" DLL_API double __cdecl GMComp(char *SrcFile, char *DstFile);
extern "C" DLL_API double __cdecl GMDecomp(char *SrcFile, char *DstFile, double Pointer);

//-- External Functions ---------------------------------------------------------
long SComp(char *SrcFile, char *DstFile, bool WithSize);
long SDecomp(char *SrcFile, char *DstFile, long Pointer, unsigned short Size);

//-- Global Variables -----------------------------------------------------------
long Result;
char LocalSrc[MAX_PATH] = "";
char LocalDst[MAX_PATH] = "";
char SrcBackUp[MAX_PATH] = "";

#endif /* _MAIN_H_ */
