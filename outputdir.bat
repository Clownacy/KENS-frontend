@ECHO off

if not exist bin mkdir bin
if not exist bin\libs mkdir bin\libs
copy KENS\*.dll bin\libs
