// Clownacy's KENS v1.4 frontend

#define WinExplorer

#include <stdio.h>
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

void checkFileExist ( char *filename )
{
	FILE *file = fopen( filename, "r" );
	if ( file == NULL )
	{
		printf( "\n  File '%s' not found\n", filename );
		exit(1);
	}
	fclose( file );
}

void handleModuleError ( bool success, char *name )
{
	if (success == false)
	{
		printf( "\n  Error initialising %s\n", name );
		checkFileExist( name );	// Check if our error is the module file being missing
		exit(1);
	}
}

void initKosinskiModule()
{
	handleModuleError( KInit(kospath), kospath );
}

void initEnigmaModule()
{
	handleModuleError( EInit(enipath), enipath );
}

void initNemesisModule()
{
	handleModuleError( NInit(nempath), nempath );
}

void initSaxmanModule()
{
	handleModuleError( SInit(saxpath), saxpath );
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
		checkFileExist( argv[2] );

		// Mode handlers
		if ( strcmp( argv[1], "kc" ) == 0 )
		{
			initKosinskiModule();
			KComp( argv[2], argv[3], false );
		}
		else if ( strcmp( argv[1], "kd" ) == 0 )
		{
			initKosinskiModule();
			KDecomp( argv[2], argv[3], 0, false );
		}
		else if ( strcmp( argv[1], "ec" ) == 0 )
		{
			initEnigmaModule();
			EComp( argv[2], argv[3], false );
		}
		else if ( strcmp( argv[1], "ed" ) == 0 )
		{
			initEnigmaModule();
			EDecomp( argv[2], argv[3], 0, false );
		}
		else if ( strcmp( argv[1], "nc" ) == 0 )
		{
			initNemesisModule();
			NComp( argv[2], argv[3] );
		}
		else if ( strcmp( argv[1], "nd" ) == 0 )
		{
			initNemesisModule();
			NDecomp( argv[2], argv[3], 0 );
		}
		else if ( strcmp( argv[1], "sc" ) == 0 )
		{
			initSaxmanModule();
			SComp( argv[2], argv[3], false );
		}
		else if ( strcmp( argv[1], "sd" ) == 0 )
		{
			initSaxmanModule();
			SDecomp( argv[2], argv[3], 0, 0 );
		}
		else
			usage( argv[0] );
		break;
	}

	// Done
	return(0);
}
