@ECHO off

echo Creating directories
if not exist bin mkdir bin

echo Compiling program
g++ -O3 kensfrontend.c KENS-source/Kosinski/GetFileSize.cpp KENS-source/Kosinski/K-Compressor.cpp KENS-source/Kosinski/K-Decompressor.cpp KENS-source/Enigma/E-Compressor.cpp KENS-source/Enigma/E-Decompressor.cpp KENS-source/Nemesis/N-Compressor.cpp KENS-source/Nemesis/N-Decompressor.cpp KENS-source/Saxman/S-Compressor.cpp KENS-source/Saxman/S-Decompressor.cpp -o bin/kensfrontend.exe

pause
