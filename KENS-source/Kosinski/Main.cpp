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

#include "Main.h"

//-----------------------------------------------------------------------------------------------
// Name: DllMain(HANDLE, DWORD ul_reason_for_call, LPVOID)
// Desc: DLL Entry Point - Performs some initializations
//-----------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE, DWORD ul_reason_for_call, LPVOID)
{
	char TempPath[MAX_PATH];
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			GetTempPath(MAX_PATH, TempPath);
			GetTempFileName(TempPath, "kos", 0, LocalSrc);
			GetTempFileName(TempPath, "kos", 0, LocalDst);
			return TRUE;
		case DLL_PROCESS_DETACH:
			DeleteFile(LocalSrc);
			DeleteFile(LocalDst);
			return TRUE;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			return TRUE;
    }
	return TRUE;
}

//-----------------------------------------------------------------------------------------------
// Name: SameSource(char *SrcFile)
// Desc: Tells whether the specified source file is the same or not
//-----------------------------------------------------------------------------------------------
bool SameSource(char *SrcFile)
{
	if (stricmp(SrcFile, SrcBackUp))
	{
		strcpy(SrcBackUp, SrcFile);
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------------------------
// Name: LoadBuffer(char *&Buffer)
// Desc: Loads the content of the destination file into a buffer and returns its size
//-----------------------------------------------------------------------------------------------
long LoadBuffer(char *&Buffer)
{
	unsigned long Size, Read;
	HANDLE hFile = CreateFile(LocalDst, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	Size = GetFileSize(hFile, NULL);
	if (Buffer==NULL) Buffer = new char[Size];
	ReadFile(hFile, Buffer, Size, &Read, NULL);
	CloseHandle(hFile);
	return Size;
}

//-----------------------------------------------------------------------------------------------
// Name: OLECHARsToChars(OLECHAR *Source, char *Destination)
// Desc: Converts a Unicode string into an ANSI string. Does not perform any initialization.
//-----------------------------------------------------------------------------------------------
int OLECHARsToChars(OLECHAR *Source, char *Destination)
{
	int Len = SysStringLen(Source);
	for (int i=0; i<Len; ++i)
	{
		Destination[i] = (char)Source[i];
	}
	Destination[i] = 0;
	return 0;
}

//-- Compression --------------------------------------------------------------------------------
long __cdecl Comp(char *SrcFile, char *DstFile, bool Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KComp(LocalSrc, LocalDst, 8192, 256, Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Compression with Sliding Windows and Recurrence Length -------------------------------------
long __cdecl CompEx(char *SrcFile, char *DstFile, long SlideWin, long RecLen, bool Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KComp(LocalSrc, LocalDst, SlideWin, RecLen, Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Decompression ------------------------------------------------------------------------------
long __cdecl Decomp(char *SrcFile, char *DstFile, long Pointer, bool Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KDecomp(LocalSrc, LocalDst, Pointer, Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Compression To Buffer ----------------------------------------------------------------------
long __cdecl CompToBuf(char *SrcFile, char *&DstBuffer, long *BufSize, bool Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KComp(LocalSrc, LocalDst, 8192, 256, Moduled);
	*BufSize = LoadBuffer(DstBuffer);
	return Result;
}

//-- Compression To Buffer with Sliding Windows and Recurrence Length ---------------------------
long __cdecl CompToBufEx(char *SrcFile, char *&DstBuffer, long *BufSize, long SlideWin, long RecLen, bool Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KComp(LocalSrc, LocalDst, SlideWin, RecLen, Moduled);
	*BufSize = LoadBuffer(DstBuffer);
	return Result;
}

//-- Decompression To Buffer --------------------------------------------------------------------
long __cdecl DecompToBuf(char *SrcFile, char *&DstBuffer, long *BufSize, long Pointer, bool Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KDecomp(LocalSrc, LocalDst, Pointer, Moduled);
	*BufSize = LoadBuffer(DstBuffer);
	return Result;
}

//-----------------------------------------------------------------------------------------------
// Name: FreeBuffer(char *&Buffer)
// Desc: Frees a buffer allocated by LoadBuffer
//-----------------------------------------------------------------------------------------------
long __cdecl FreeBuffer(char *&Buffer)
{
	delete[] Buffer;
	Buffer = NULL;
	return 0;
}

//-- Visual Basic Compression -------------------------------------------------------------------
long __stdcall VBComp(VARIANT _SrcFile, VARIANT _DstFile, bool Moduled)
{
	char SrcFile[MAX_PATH]; OLECHARsToChars(_SrcFile.bstrVal, SrcFile);
	char DstFile[MAX_PATH]; OLECHARsToChars(_DstFile.bstrVal, DstFile);
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KComp(LocalSrc, LocalDst, 8192, 256, Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Visual Basic Compression with Sliding Windows and Recurrence Length ------------------------
long __stdcall VBCompEx(VARIANT _SrcFile, VARIANT _DstFile, long SlideWin, long RecLen, bool Moduled)
{
	char SrcFile[MAX_PATH]; OLECHARsToChars(_SrcFile.bstrVal, SrcFile);
	char DstFile[MAX_PATH]; OLECHARsToChars(_DstFile.bstrVal, DstFile);
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KComp(LocalSrc, LocalDst, SlideWin, RecLen, Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Visual Basic Decompression -----------------------------------------------------------------
long __stdcall VBDecomp(VARIANT _SrcFile, VARIANT _DstFile, long Pointer, bool Moduled)
{
	char SrcFile[MAX_PATH]; OLECHARsToChars(_SrcFile.bstrVal, SrcFile);
	char DstFile[MAX_PATH]; OLECHARsToChars(_DstFile.bstrVal, DstFile);
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KDecomp(LocalSrc, LocalDst, Pointer, Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Game Maker Compression ---------------------------------------------------------------------
double __cdecl GMComp(char *SrcFile, char *DstFile, double Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KComp(LocalSrc, LocalDst, 8192, 256, (bool)Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Game Maker Decompression -------------------------------------------------------------------
double __cdecl GMDecomp(char *SrcFile, char *DstFile, double Pointer, double Moduled)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = KDecomp(LocalSrc, LocalDst, (long)Pointer, (bool)Moduled);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}
