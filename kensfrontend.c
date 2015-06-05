// Clownacy's KENS v1.4 frontend

#include <stdio.h>
//#include <stdbool.h>	// use with C compiler
#include <sys/stat.h>
#include <libgen.h>
#include "KENS.h"

extern "C" {
	/*
	argv[0] = Program name
	argv[1] = Mode
	argv[2] = Input path
	argv[3] = Output path
	*/

void usage ( char *programpath )
{
	char *programname = basename( programpath );
	printf( "Frontend made by Clownacy\n\n  Usage:  %s [mode] [inputdir] [outputdir]\n\n  Modes:\n   kc - Kosinski (compress)\n   kd - Kosinski (decompress)\n   ec - Enigma (compress)\n   ed - Enigma (decompress)\n   nc - Nemesis (compress)\n   nd - Nemesis (decompress)\n   sc - Saxman (compress)\n   sd - Saxman (decompress)\n", programname );
}

bool checkFileExist ( char *filepath )
{
	struct stat buffer;
	bool exist = ( stat(filepath, &buffer) == 0 );
	if ( !exist )
	{
		char *filename = basename( filepath );
		printf( "\n  File '%s' not found\n", filename );
	}
	return exist;
}

void processFile ( char *argv[1] )
{
	if ( strcmp( argv[1], "kc" ) == 0 )
		KComp( argv[2], argv[3], 8192, 256, false );
	else if ( strcmp( argv[1], "kd" ) == 0 )
		KDecomp( argv[2], argv[3], 0, false );
	else if ( strcmp( argv[1], "ec" ) == 0 )
		EComp( argv[2], argv[3], false );
	else if ( strcmp( argv[1], "ed" ) == 0 )
		EDecomp( argv[2], argv[3], 0, false );
	else if ( strcmp( argv[1], "nc" ) == 0 )
		NComp( argv[2], argv[3] );
	else if ( strcmp( argv[1], "nd" ) == 0 )
		NDecomp( argv[2], argv[3], 0 );
	else if ( strcmp( argv[1], "sc" ) == 0 )
		SComp( argv[2], argv[3], false );
	else if ( strcmp( argv[1], "sd" ) == 0 )
		SDecomp( argv[2], argv[3], 0, 0 );
	else
		usage( argv[0] );
}

int main ( int argc, char *argv[1] )
{
	switch ( argc )
	{
	default:
		usage( argv[0] );
		break;

	#ifdef _WIN32
		// If opened by Explorer, don't close immediately
		case 1:
			usage( argv[0] );
			getchar();
			break;
	#endif

	case 4:
		if ( checkFileExist( argv[2] ) == false )
			break;

		processFile( argv );
	}

	// Done
	return 0;
}

}
