@ECHO off

echo|set /p=Creating directories...
if not exist bin mkdir bin
if not exist bin\libs mkdir bin\libs
echo done

echo|set /p=Compiling program...
g++ -O3 kensfrontend.c -o bin/kensfrontend.exe
echo done

echo.
echo Copying files
echo.
echo|set /p=Enigma.dll...
copy KENS\Enigma.dll bin\libs >NUL
echo done
echo.
echo|set /p=Kosinski.dll...
copy KENS\Kosinski.dll bin\libs >NUL
echo done
echo.
echo|set /p=Nemesis.dll...
copy KENS\Nemesis.dll bin\libs >NUL
echo done
echo.
echo|set /p=Saxman.dll...
copy KENS\Saxman.dll bin\libs >NUL
echo done
echo.

pause
