@ECHO off

echo Creating directories
if not exist bin mkdir bin
if not exist bin\libs mkdir bin\libs

echo Compiling program
g++ -O3 kensfrontend.c KENS-source/Kosinski/GetFileSize.cpp KENS-source/Kosinski/K-Compressor.cpp KENS-source/Kosinski/K-Decompressor.cpp -o bin/kensfrontend.exe

echo.
echo Copying files...
echo.
echo Enigma.dll
copy KENS\Enigma.dll bin\libs
echo.
echo Nemesis.dll
copy KENS\Nemesis.dll bin\libs
echo.
echo Saxman.dll
copy KENS\Saxman.dll bin\libs
echo.

pause
