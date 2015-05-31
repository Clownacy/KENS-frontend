{---------------------------------------------------------------------------*\
|                                                                            |
| Kosinski.dll, Enigma.dll, Nemesis.dll, Saxman.dll: Compression and         |
| Decompression of data in Kosinski, Enimga, Nemesis and Saxman formats      |
| Copyright © 2002-2004 The KENS Project Development Team                    |
|                                                                            |
| This library is free software; you can redistribute it and/or              |
| modify it under the terms of the GNU Lesser General Public                 |
| License as published by the Free Software Foundation; either               |
| version 2.1 of the License, or (at your option) any later version.         |
|                                                                            |
| This library is distributed in the hope that it will be useful,            |
| but WITHOUT ANY WARRANTY; without even the implied warranty of             |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          |
| Lesser General Public License for more details.                            |
|                                                                            |
| You should have received a copy of the GNU Lesser General Public           |
| License along with this library; if not, write to the Free Software        |
| Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  |
|                                                                            |
\*---------------------------------------------------------------------------}
unit KENS;

{ C Header to Delphi Unit conversion by S.O. (amarokorama@msn.com)
  Edited by Magus (added support for functions that output to a buffer)
  Corrected by S.O
  Version 1.2, 09.02.2004

  Note: There's one compiler switch, "KENS_LoadDynamically".
  If it is defined, Delphi KENS works just like the C version. That means you
  have to call "XInit" in order to load the DLL before you are safe to use any
  of the "XComp", "XDecomp" funtions. However, it should be more convenient
  to use the helper function "XAvailable" (which itself calls "XInit" but only
  if necessary).
  If "KENS_LoadDynamically" is not defined, then the KENS DLLs that are used by
  your Delphi program are statically imported, which means that your program
  won't start up if one of the used DLLs is missing. With "KENS_LoadDynamically"
  disabled, it's safe to use the "XComp" and "XDecomp" anytime, a call to
  "XInit" or "XAvailable" is not necessary. However, you are free to call
  "XInit" or "XAvailable" anyway, in this case they are just dummies that
  always return "True". That way, you don't need to change your code,
  regardless whether you choose to statically or dynamically import the
  KENS DLLs.

  Examples:
  1)
    SrcFileName := OpenDialog.FileName;
    if KComp(PChar(SrcFileName), 'Kosinski.bin', False) <> SUCCESS then
      ShowMessage('ERROR! KComp failed!');

  2)
    if KAvailable then
    begin
      SrcFileName := OpenDialog.FileName;
      if KComp(PChar(SrcFileName), 'Kosinski.bin', False) <> SUCCESS then
        ShowMessage('ERROR! KComp failed!');
    end
    else ShowMessage('ERROR! Kosinski functions are not available!');

  Example 1) is only safe when "KENS_LoadDynamically" is NOT defined, because
  neither KInit nor KAvailable are called.
  Example 2) is ALWAYS fine, with or without "KENS_LoadDynamically". So this
  is the recommended way.


  And finally, here's an example for how to use the buffer based functions:

  function DecompKosinskiToString(const InFile: string): string;
  var
    PBuf: Pointer;
    BufSize: Longint;
  begin
    // decompress InFile and store the data in a string
    // (eg. for a Kosinski compressed text file)
    Result := '';
    if KAvailable then
    begin
      // if we want the DLL to allocate the buffer, we must pass nil
      PBuf := nil;
      if KDecompToBuf(PChar(InFile), PBuf, BufSize, 0, False) = SUCCESS then
      try
        // set the return string to the right length
        SetLength(Result, BufSize);
        // copy data from buffer into string
        Move(PBuf^, Result[1], BufSize);
      finally
        // let the DLL free the memory it allocated for us
        KFreeBuffer(PBuf);
        // PBuf is now set to nil
      end;
    end;
  end;

 }


interface

// Compiler switch KENS_LoadDynamically is defined by default
{$DEFINE KENS_LoadDynamically}


{$IFDEF KENS_LoadDynamically}

uses
  Windows;


var
  KComp:        function (SrcFile, DstFile: PChar; Moduled: Boolean): Longint; cdecl;
  KDecomp:      function (SrcFile, DstFile: PChar; SrcFileOffset: Longint; Moduled: Boolean): Longint; cdecl;
  KCompEx:      function (SrcFile, DstFile: PChar; SlideWin, RecLen: Longint; Moduled: Boolean): Longint; cdecl;
  KCompToBuf:   function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; Moduled: Boolean): Longint; cdecl;
  KDecompToBuf: function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset: Longint; Moduled: Boolean): Longint; cdecl;
  KCompToBufEx: function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SlideWin, RecLen: Longint; Moduled: Boolean): Longint; cdecl;
  KFreeBuffer:  function (var Buffer: Pointer): Longint; cdecl;

  EComp:        function (SrcFile, DstFile: PChar; Padding: Boolean): Longint; cdecl;
  EDecomp:      function (SrcFile, DstFile: PChar; SrcFileOffset: Longint; Padding: Boolean): Longint; cdecl;
  ECompToBuf:   function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; Padding: Boolean): Longint; cdecl;
  EDecompToBuf: function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset: Longint; Padding: Boolean): Longint; cdecl;
  EFreeBuffer:  function (var Buffer: Pointer): Longint; cdecl;

  NComp:        function (SrcFile, DstFile: PChar): Longint; cdecl;
  NDecomp:      function (SrcFile, DstFile: PChar; SrcFileOffset: Longint): Longint; cdecl;
  NCompToBuf:   function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint): Longint; cdecl;
  NDecompToBuf: function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset: Longint): Longint; cdecl;
  NFreeBuffer:  function (var Buffer: Pointer): Longint; cdecl;

  SComp:        function (SrcFile, DstFile: PChar; WithSize: Boolean): Longint; cdecl;
  SDecomp:      function (SrcFile, DstFile: PChar; SrcFileOffset, Size: Longint): Longint; cdecl;
  SCompToBuf:   function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; WithSize: Boolean): Longint; cdecl;
  SDecompToBuf: function (SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset, Size: Longint): Longint; cdecl;
  SFreeBuffer:  function (var Buffer: Pointer): Longint; cdecl;

{$ELSE}

// Kosinski.dll
function KComp(SrcFile, DstFile: PChar; Moduled: Boolean): Longint; cdecl;
function KDecomp(SrcFile, DstFile: PChar; SrcFileOffset: Longint; Moduled: Boolean): Longint; cdecl;
function KCompEx(SrcFile, DstFile: PChar; SlideWin, RecLen: Longint; Moduled: Boolean): Longint; cdecl;
function KCompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; Moduled: Boolean): Longint; cdecl;
function KDecompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset: Longint; Moduled: Boolean): Longint; cdecl;
function KCompToBufEx(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SlideWin, RecLen: Longint; Moduled: Boolean): Longint; cdecl;
function KFreeBuffer(var Buffer: Pointer): Longint; cdecl;

// Enigma.dll
function EComp(SrcFile, DstFile: PChar; Padding: Boolean): Longint; cdecl;
function EDecomp(SrcFile, DstFile: PChar; SrcFileOffset: Longint; Padding: Boolean): Longint; cdecl;
function ECompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; Padding: Boolean): Longint; cdecl;
function EDecompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset: Longint; Padding: Boolean): Longint; cdecl;
function EFreeBuffer(var Buffer: Pointer): Longint; cdecl;

// Nemesis.dll
function NComp(SrcFile, DstFile: PChar): Longint; cdecl;
function NDecomp(SrcFile, DstFile: PChar; SrcFileOffset: Longint): Longint; cdecl;
function NCompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint): Longint; cdecl;
function NDecompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset: Longint): Longint; cdecl;
function NFreeBuffer(var Buffer: Pointer): Longint; cdecl;

// Saxman.dll
function SComp(SrcFile, DstFile: PChar; WithSize: Boolean): Longint; cdecl;
function SDecomp(SrcFile, DstFile: PChar; SrcFileOffset, Size: Longint): Longint; cdecl;
function SCompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; WithSize: Boolean): Longint; cdecl;
function SDecompToBuf(SrcFile: PChar; var DstBuffer: Pointer; out BufSize: Longint; SrcFileOffset, Size: Longint): Longint; cdecl;
function SFreeBuffer(var Buffer: Pointer): Longint; cdecl;

{$ENDIF}



function KInit(DLLName: string = ''): Boolean;
function EInit(DLLName: string = ''): Boolean;
function NInit(DLLName: string = ''): Boolean;
function SInit(DLLName: string = ''): Boolean;

// functions introduced by S.O. for convenience, use these instead of XInit
function KAvailable: Boolean;
function EAvailable: Boolean;
function NAvailable: Boolean;
function SAvailable: Boolean;



const
  // some return values
  SUCCESS                           = $00;
  ERROR_UNKNOWN                     = $01;
  ERROR_SOURCE_FILE_DOES_NOT_EXIST  = $02;
  ERROR_MODULED_GREATHER_THAN_65535 = $03;  // used only by KComp


  // default DLL names
  Kosinski = 'Kosinski.dll';
  Enigma   = 'Enigma.dll';
  Nemesis  = 'Nemesis.dll';
  Saxman   = 'Saxman.dll';
  // DLL Proc Names
  CompProcName         = 'Comp';
  Comp2ProcName        = 'Comp2';
  CompToBufProcName    = 'CompToBuf';
  CompToBuf2ProcName   = 'CompToBuf2';
  DecompProcName       = 'Decomp';
  Decomp2ProcName      = 'Decomp2';
  DecompToBufProcName  = 'DecompToBuf';
  DecompToBuf2ProcName = 'DecompToBuf2';
  CompExProcName       = 'CompEx';
  CompToBufExProcName  = 'CompToBufEx';
  FreeBufferProcName   = 'FreeBuffer';


implementation


{$IFDEF KENS_LoadDynamically}

var
  hKosinski,
  hEnigma,
  hNemesis,
  hSaxman: HMODULE;



function CheckPointers(const Pointers: array of Pointer): Boolean;
var
  i: Integer;
begin
  Result := False;
  // if any one of the pointers is nil, then return False
  for i := Low(Pointers) to High(Pointers) do
    if not Assigned(Pointers[i]) then Exit;
  Result := True;
end;



function KInit(DLLName: string): Boolean;
begin
  if DLLName = '' then DLLName := Kosinski;
  // only try to load DLL when it isn't already loaded
  if hKosinski = 0 then hKosinski := LoadLibrary(PChar(DLLName));
  Result := (hKosinski <> 0);
  if Result then
  begin
    KComp        := GetProcAddress(hKosinski, CompProcName);
    KDecomp      := GetProcAddress(hKosinski, DecompProcName);
    KCompEx      := GetProcAddress(hKosinski, CompExProcName);
    KCompToBuf   := GetProcAddress(hKosinski, CompToBufProcName);
    KDecompToBuf := GetProcAddress(hKosinski, DecompToBufProcName);
    KCompToBufEx := GetProcAddress(hKosinski, CompToBufExProcName);
    KFreeBuffer  := GetProcAddress(hKosinski, FreeBufferProcName);
    Result := CheckPointers([@KComp, @KDecomp, @KCompEx, @KCompToBuf,
      @KDecompToBuf, @KCompToBufEx, @KFreeBuffer]);
  end;
end;



function EInit(DLLName: string): Boolean;
begin
  if DLLName = '' then DLLName := Enigma;
  // only try to load DLL when it isn't already loaded
  if hEnigma = 0 then hEnigma := LoadLibrary(PChar(DLLName));
  Result := (hEnigma <> 0);
  if Result then
  begin
    EComp        := GetProcAddress(hEnigma, CompProcName);
    EDecomp      := GetProcAddress(hEnigma, DecompProcName);
    ECompToBuf   := GetProcAddress(hEnigma, CompToBufProcName);
    EDecompToBuf := GetProcAddress(hEnigma, DecompToBufProcName);
    EFreeBuffer  := GetProcAddress(hEnigma, FreeBufferProcName);
    Result := CheckPointers([@EComp, @EDecomp, @ECompToBuf, @EDecompToBuf,
      @EFreeBuffer]);
  end;
end;



function NInit(DLLName: string): Boolean;
begin
  if DLLName = '' then DLLName := Nemesis;
  // only try to load DLL when it isn't already loaded
  if hNemesis = 0 then hNemesis := LoadLibrary(PChar(DLLName));
  Result := (hNemesis <> 0);
  if Result then
  begin
    NComp        := GetProcAddress(hNemesis, CompProcName);
    NDecomp      := GetProcAddress(hNemesis, DecompProcName);
    NCompToBuf   := GetProcAddress(hNemesis, CompToBufProcName);
    NDecompToBuf := GetProcAddress(hNemesis, DecompToBufProcName);
    NFreeBuffer  := GetProcAddress(hNemesis, FreeBufferProcName);
    Result := CheckPointers([@NComp, @NDecomp, @NCompToBuf, @NDecompToBuf,
      @NFreeBuffer]);
  end;
end;



function SInit(DLLName: string): Boolean;
begin
  if DLLName = '' then DLLName := Saxman;
  // only try to load DLL when it isn't already loaded
  if hSaxman = 0 then hSaxman := LoadLibrary(PChar(DLLName));
  Result := (hSaxman <> 0);
  if Result then
  begin
    SComp        := GetProcAddress(hSaxman, Comp2ProcName);
    SDecomp      := GetProcAddress(hSaxman, Decomp2ProcName);
    SCompToBuf   := GetProcAddress(hSaxman, CompToBuf2ProcName);
    SDecompToBuf := GetProcAddress(hSaxman, DecompToBuf2ProcName);
    SFreeBuffer  := GetProcAddress(hSaxman, FreeBufferProcName);
    Result := CheckPointers([@SComp, @SDecomp, @SCompToBuf, @SDecompToBuf,
      @SFreeBuffer]);
  end;
end;



function KAvailable: Boolean;
begin
  Result := (hKosinski <> 0);
  if not Result then Result := KInit;
end;

function EAvailable: Boolean;
begin
  Result := (hEnigma <> 0);
  if not Result then Result := EInit;
end;

function NAvailable: Boolean;
begin
  Result := (hNemesis <> 0);
  if not Result then Result := NInit;
end;

function SAvailable: Boolean;
begin
  Result := (hSaxman <> 0);
  if not Result then Result := SInit;
end;



{$ELSE}

{ Externals from Kosinski.dll }

function KComp; external Kosinski name CompProcName;
function KDecomp; external Kosinski name DecompProcName;
function KCompEx; external Kosinski name CompExProcName;
function KCompToBuf; external Kosinski name CompToBufProcName;
function KDecompToBuf; external Kosinski name DecompToBufProcName;
function KCompToBufEx; external Kosinski name CompToBufExProcName;
function KFreeBuffer; external Kosinski name FreeBufferProcName;

{ Externals from Enigma.dll }

function EComp; external Enigma name CompProcName;
function EDecomp; external Enigma name DecompProcName;
function ECompToBuf; external Enigma name CompToBufProcName;
function EDecompToBuf; external Enigma name DecompToBufProcName;
function EFreeBuffer; external Enigma name FreeBufferProcName;

{ Externals from Nemesis.dll }

function NComp; external Nemesis name CompProcName;
function NDecomp; external Nemesis name DecompProcName;
function NCompToBuf; external Nemesis name CompToBufProcName;
function NDecompToBuf; external Nemesis name DecompToBufProcName;
function NFreeBuffer; external Nemesis name FreeBufferProcName;

{ Externals from Saxman.dll }

function SComp; external Saxman name Comp2ProcName;
function SDecomp; external Saxman name Decomp2ProcName;
function SCompToBuf; external Saxman name CompToBuf2ProcName;
function SDecompToBuf; external Saxman name DecompToBuf2ProcName;
function SFreeBuffer; external Saxman name FreeBufferProcName;



{ XInit dummy functions }

function KInit(DLLName: string): Boolean;
begin
  Result := True;
end;

function EInit(DLLName: string): Boolean;
begin
  Result := True;
end;

function NInit(DLLName: string): Boolean;
begin
  Result := True;
end;

function SInit(DLLName: string): Boolean;
begin
  Result := True;
end;

{ XAvailable dummy functions }

function KAvailable: Boolean;
begin
  Result := True;
end;

function EAvailable: Boolean;
begin
  Result := True;
end;

function NAvailable: Boolean;
begin
  Result := True;
end;

function SAvailable: Boolean;
begin
  Result := True;
end;


{$ENDIF}


end.

