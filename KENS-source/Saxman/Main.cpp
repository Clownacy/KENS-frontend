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
			GetTempFileName(TempPath, "sax", 0, LocalSrc);
			GetTempFileName(TempPath, "sax", 0, LocalDst);
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

//-- Compression with size optionally written to output or discarded ----------------------------
long __cdecl Comp2(char *SrcFile, char *DstFile, bool WithSize)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SComp(LocalSrc, LocalDst, WithSize);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Decompression with size optionally included in input or set by the user --------------------
long __cdecl Decomp2(char *SrcFile, char *DstFile, long Pointer, unsigned short Size)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SDecomp(LocalSrc, LocalDst, Pointer, Size);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Compression To Buffer with size optionally written to output or discarded ------------------
long __cdecl CompToBuf2(char *SrcFile, char *&DstBuffer, long *BufSize, bool WithSize)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SComp(LocalSrc, LocalDst, WithSize);
	*BufSize = LoadBuffer(DstBuffer);
	return Result;
}

//-- Decompression To Buffer with size optionally included in input or set by the user ----------
long __cdecl DecompToBuf2(char *SrcFile, char *&DstBuffer, long *BufSize, long Pointer, unsigned short Size)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SDecomp(LocalSrc, LocalDst, Pointer, Size);
	*BufSize = LoadBuffer(DstBuffer);
	return Result;
}

//-----------------------------------------------------------------------------------------------
// Name: FreeBuffer(char *Buffer)
// Desc: Frees a buffer allocated by LoadBuffer
//-----------------------------------------------------------------------------------------------
long __cdecl FreeBuffer(char *&Buffer)
{
	delete[] Buffer;
	Buffer = NULL;
	return 0;
}

//-- Visual Basic Compression with size optionally written to output or discarded ---------------
long __stdcall VBComp2(VARIANT _SrcFile, VARIANT _DstFile, bool WithSize)
{
	char SrcFile[MAX_PATH]; OLECHARsToChars(_SrcFile.bstrVal, SrcFile);
	char DstFile[MAX_PATH]; OLECHARsToChars(_DstFile.bstrVal, DstFile);
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SComp(LocalSrc, LocalDst, WithSize);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Visual Basic Decompression with size optionally included in input or set by the user -------
long __stdcall VBDecomp2(VARIANT _SrcFile, VARIANT _DstFile, long Pointer, long Size)
{
	char SrcFile[MAX_PATH]; OLECHARsToChars(_SrcFile.bstrVal, SrcFile);
	char DstFile[MAX_PATH]; OLECHARsToChars(_DstFile.bstrVal, DstFile);
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SDecomp(LocalSrc, LocalDst, Pointer, (unsigned short)Size);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Game Maker Compression with size optionally written to output or discarded -----------------
double __cdecl GMComp2(char *SrcFile, char *DstFile, double WithSize)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SComp(LocalSrc, LocalDst, bool(WithSize));
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Game Maker Decompression with size optionally included in input or set by the user ---------
double __cdecl GMDecomp2(char *SrcFile, char *DstFile, double Pointer, double Size)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SDecomp(LocalSrc, LocalDst, (long)Pointer, (unsigned short)Size);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Compression with size written to output ----------------------------------------------------
long __cdecl Comp(char *SrcFile, char *DstFile)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SComp(LocalSrc, LocalDst, true);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Decompression with size included in input --------------------------------------------------
long __cdecl Decomp(char *SrcFile, char *DstFile, long Pointer)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SDecomp(LocalSrc, LocalDst, Pointer, 0);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Visual Basic Compression with size written to output ---------------------------------------
long __stdcall VBComp(VARIANT _SrcFile, VARIANT _DstFile)
{
	char SrcFile[MAX_PATH]; OLECHARsToChars(_SrcFile.bstrVal, SrcFile);
	char DstFile[MAX_PATH]; OLECHARsToChars(_DstFile.bstrVal, DstFile);
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SComp(LocalSrc, LocalDst, true);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Visual Basic Decompression with size included in input -------------------------------------
long __stdcall VBDecomp(VARIANT _SrcFile, VARIANT _DstFile, long Pointer)
{
	char SrcFile[MAX_PATH]; OLECHARsToChars(_SrcFile.bstrVal, SrcFile);
	char DstFile[MAX_PATH]; OLECHARsToChars(_DstFile.bstrVal, DstFile);
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SDecomp(LocalSrc, LocalDst, Pointer, 0);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Game Maker Compression with size written to output -----------------------------------------
double __cdecl GMComp(char *SrcFile, char *DstFile)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SComp(LocalSrc, LocalDst, true);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}

//-- Game Maker Decompression with size included in input ---------------------------------------
double __cdecl GMDecomp(char *SrcFile, char *DstFile, double Pointer)
{
	if (!SameSource(SrcFile)) CopyFile(SrcFile, LocalSrc, FALSE);
	Result = SDecomp(LocalSrc, LocalDst, (long)Pointer, 0);
	CopyFile(LocalDst, DstFile, FALSE);
	return Result;
}
