// Clownacy's KENS v1.4 frontend

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <libgen.h>
#include "KENS/Include/KENS.h"
	/*
	argv[0] = Program name
	argv[1] = Mode
	argv[2] = Input path
	argv[3] = Output path
	*/

char* programpath;
char* settings;
char* inputpath;
char* outputpath;

const char* const kospath = "libs/Kosinski.dll";
const char* const enipath = "libs/Enigma.dll";
const char* const nempath = "libs/Nemesis.dll";
const char* const saxpath = "libs/Saxman.dll";

void usage(void)
{
	char* programname = basename(programpath);
	printf("Frontend made by Clownacy\n\n  Usage:  %s [mode] [inputdir] [outputdir]\n\n  Modes:\n   kc - Kosinski (compress)\n   kd - Kosinski (decompress)\n   ec - Enigma (compress)\n   ed - Enigma (decompress)\n   nc - Nemesis (compress)\n   nd - Nemesis (decompress)\n   sc - Saxman (compress)\n   sd - Saxman (decompress)\n", programname);
}

bool checkFileExist(const char* const filepath)
{
	struct stat buffer;
	bool exist = (stat(filepath, &buffer) == 0);
	if (!exist)
	{
		char* filename = (char*)malloc(strlen(filepath)+1);	// The '+1' is for the null-character
		strcpy(filename, filepath);
		filename = basename(filename);
		printf("\n  File '%s' not found\n", filename);
		free(filename);
	}
	return exist;
}

bool handleModuleError(bool success, const char* const name)
{
	if (success == false)
	{
		printf("\n  Error initialising %s\n", name);
		checkFileExist(name);	// Check if our error is the module file being missing
	}
	return success;
}

bool initKosinskiModule(void)
{
	return handleModuleError(KInit(kospath), kospath);
}

bool initEnigmaModule(void)
{
	return handleModuleError(EInit(enipath), enipath);
}

bool initNemesisModule(void)
{
	return handleModuleError(NInit(nempath), nempath);
}

bool initSaxmanModule(void)
{
	return handleModuleError(SInit(saxpath), saxpath);
}

void processFile(void)
{
	if (strcmp(settings, "kc") == 0)
		if (initKosinskiModule() == true)
			KComp(inputpath, outputpath, false);
	else if (strcmp( settings, "kd") == 0)
		if (initKosinskiModule() == true)
			KDecomp(inputpath, outputpath, 0, false);
	else if (strcmp(settings, "ec") == 0)
		if (initEnigmaModule() == true)
			EComp(inputpath, outputpath, false);
	else if (strcmp(settings, "ed") == 0)
		if (initEnigmaModule() == true)
			EDecomp(inputpath, outputpath, 0, false);
	else if (strcmp(settings, "nc") == 0)
		if (initNemesisModule() == true)
			NComp(inputpath, outputpath);
	else if (strcmp(settings, "nd") == 0)
		if (initNemesisModule() == true)
			NDecomp(inputpath, outputpath, 0);
	else if (strcmp(settings, "sc") == 0)
		if (initSaxmanModule() == true)
			SComp(inputpath, outputpath, false);
	else if (strcmp(settings, "sd") == 0)
		if (initSaxmanModule() == true)
			SDecomp(inputpath, outputpath, 0, 0);
	else
		usage();
}

int main(int argc, char* argv[])
{
	programpath = argv[0];
	settings = argv[1];
	inputpath = argv[2];
	outputpath = argv[3];

	switch(argc)
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
		if(checkFileExist(inputpath) == false)
			break;

		processFile();
		break;
	}

	// Done
	return 0;
}

#ifdef __cplusplus
}
#endif
