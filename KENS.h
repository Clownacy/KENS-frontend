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

long KComp(char *SrcFile, char *DstFile, int SlideWin, int RecLen, bool Moduled);
long KDecomp(char *SrcFile, char *DstFile, long Pointer, bool Moduled);

long EComp(char *SrcFile, char *DstFile, bool Padding);
long EDecomp(char *SrcFile, char *DstFile, long Pointer, bool Padding);

long NComp(char *SrcFile, char *DstFile);
long NDecomp(char *SrcFile, char *DstFile, long Pointer);

long SComp(char *SrcFile, char *DstFile, bool WithSize);
long SDecomp(char *SrcFile, char *DstFile, long Pointer, unsigned short Size);


#endif // _KENS_H_
