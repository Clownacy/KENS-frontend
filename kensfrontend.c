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

char *programpath;
char *settings;
char *inputpath;
char *outputpath;

void usage()
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

void processFile()
{
	if ( strcmp( settings, "kc" ) == 0 )
		KComp( inputpath, outputpath, 8192, 256, false );
	else if ( strcmp( settings, "kd" ) == 0 )
		KDecomp( inputpath, outputpath, 0, false );
	else if ( strcmp( settings, "ec" ) == 0 )
		EComp( inputpath, outputpath, false );
	else if ( strcmp( settings, "ed" ) == 0 )
		EDecomp( inputpath, outputpath, 0, false );
	else if ( strcmp( settings, "nc" ) == 0 )
		NComp( inputpath, outputpath );
	else if ( strcmp( settings, "nd" ) == 0 )
		NDecomp( inputpath, outputpath, 0 );
	else if ( strcmp( settings, "sc" ) == 0 )
		SComp( inputpath, outputpath, false );
	else if ( strcmp( settings, "sd" ) == 0 )
		SDecomp( inputpath, outputpath, 0, 0 );
	else
		usage();
}

int main ( int argc, char *argv[1] )
{
	programpath = argv[0];
	settings = argv[1];
	inputpath = argv[2];
	outputpath = argv[3];

	switch ( argc )
	{
	default:
		usage();
		break;

	#ifdef _WIN32
		// If opened by Explorer, don't close immediately
		case 1:
			usage();
			getchar();
			break;
	#endif

	case 4:
		if ( checkFileExist( inputpath ) == false )
			break;

		processFile();
	}

	// Done
	return 0;
}

}
