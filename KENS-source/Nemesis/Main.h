/*-----------------------------------------------------------------------------*\
|																				|
|	Nemesis.dll: Compression / Decompression of data in Nemesis format			|
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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

//-- Functions to Export --------------------------------------------------------
extern "C" DLL_API long __cdecl Comp(char *SrcFile, char *DstFile);
extern "C" DLL_API long __cdecl Decomp(char *SrcFile, char *DstFile, long Pointer);
extern "C" DLL_API long __cdecl CompToBuf(char *SrcFile, char *&DstBuffer, long *BufSize);
extern "C" DLL_API long __cdecl DecompToBuf(char *SrcFile, char *&DstBuffer, long *BufSize, long Pointer);
extern "C" DLL_API long __cdecl FreeBuffer(char *&Buffer);
extern "C" DLL_API long __stdcall VBComp(VARIANT SrcFile, VARIANT DstFile);
extern "C" DLL_API long __stdcall VBDecomp(VARIANT SrcFile, VARIANT DstFile, long Pointer);
extern "C" DLL_API double __cdecl GMComp(char *SrcFile, char *DstFile);
extern "C" DLL_API double __cdecl GMDecomp(char *SrcFile, char *DstFile, double Pointer);

//-- External Functions ---------------------------------------------------------
long NComp(char *SrcFile, char *DstFile);
long NDecomp(char *SrcFile, char *DstFile, long Pointer);

//-- Global Variables -----------------------------------------------------------
long Result;
char LocalSrc[MAX_PATH] = "";
char LocalDst[MAX_PATH] = "";
char SrcBackUp[MAX_PATH] = "";

#endif /* _MAIN_H_ */