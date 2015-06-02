// Clownacy's KENS v1.4 frontend

#define WinExplorer

#include <stdio.h>
//#include <stdbool.h>	// use with C compiler
#include <string.h>	// use with C++ compiler
#include "KENS/Include/KENS.h"

	/*
	argv[0] = Program name
	argv[1] = Mode
	argv[2] = Input path
	argv[3] = Output path
	*/

char kospath[] = "libs/Kosinski.dll";
char enipath[] = "libs/Enigma.dll";
char nempath[] = "libs/Nemesis.dll";
char saxpath[] = "libs/Saxman.dll";

void usage ( char programName[] )
{
	printf( "Frontend made by Clownacy\n\n  Usage:  %s [mode] [inputdir] [outputdir]\n\n  Modes:\n   kc - Kosinski (compress)\n   kd - Kosinski (decompress)\n   ec - Enigma (compress)\n   ed - Enigma (decompress)\n   nc - Nemesis (compress)\n   nd - Nemesis (decompress)\n   sc - Saxman (compress)\n   sd - Saxman (decompress)\n", programName );
}

bool checkFileExist ( char *filename )
{
	FILE *file = fopen( filename, "r" );
	if ( file == NULL )
	{
		printf( "\n  File '%s' not found\n", filename );
		return false;
	}
	else
		return true;
	fclose( file );
}

bool handleModuleError ( bool success, char *name )
{
	if (success == false)
	{
		printf( "\n  Error initialising %s\n", name );
		checkFileExist( name );	// Check if our error is the module file being missing
	}
	return success;
}

bool initKosinskiModule()
{
	return handleModuleError( KInit(kospath), kospath );
}

bool initEnigmaModule()
{
	return handleModuleError( EInit(enipath), enipath );
}

bool initNemesisModule()
{
	return handleModuleError( NInit(nempath), nempath );
}

bool initSaxmanModule()
{
	return handleModuleError( SInit(saxpath), saxpath );
}

int main ( int argc, char *argv[1] )
{
	switch ( argc )
	{
	default:
		usage( argv[0] );
		break;

	#ifdef WinExplorer
		// If opened by Explorer, don't close immediately
		case 1:
			usage( argv[0] );
			getchar();
			break;
	#endif

	case 4:
		if ( checkFileExist( argv[2] ) == false )
			break;

		// Mode handlers
		if ( strcmp( argv[1], "kc" ) == 0 )
		{
			if ( initKosinskiModule() == false )
				break;
			KComp( argv[2], argv[3], false );
		}
		else if ( strcmp( argv[1], "kd" ) == 0 )
		{
			if ( initKosinskiModule() == false )
				break;
			KDecomp( argv[2], argv[3], 0, false );
		}
		else if ( strcmp( argv[1], "ec" ) == 0 )
		{
			if ( initEnigmaModule() == false )
				break;
			EComp( argv[2], argv[3], false );
		}
		else if ( strcmp( argv[1], "ed" ) == 0 )
		{
			if ( initEnigmaModule() == false )
				break;
			EDecomp( argv[2], argv[3], 0, false );
		}
		else if ( strcmp( argv[1], "nc" ) == 0 )
		{
			if ( initNemesisModule() == false )
				break;
			NComp( argv[2], argv[3] );
		}
		else if ( strcmp( argv[1], "nd" ) == 0 )
		{
			if ( initNemesisModule() == false )
				break;
			NDecomp( argv[2], argv[3], 0 );
		}
		else if ( strcmp( argv[1], "sc" ) == 0 )
		{
			if ( initSaxmanModule() == false )
				break;
			SComp( argv[2], argv[3], false );
		}
		else if ( strcmp( argv[1], "sd" ) == 0 )
		{
			if ( initSaxmanModule() == false )
				break;
			SDecomp( argv[2], argv[3], 0, 0 );
		}
		else
			usage( argv[0] );
		break;
	}

	// Done
	return(0);
}
