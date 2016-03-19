// KENS v1.4 frontend
// Copyright © 2015-2016 Clownacy

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

char* program_path;
char* settings;
char* input_path;
char* output_path;

const char* const kosinski_path = "libs/Kosinski.dll";
const char* const enigma_path = "libs/Enigma.dll";
const char* const nemesis_path = "libs/Nemesis.dll";
const char* const saxman_path = "libs/Saxman.dll";

void usage(void)
{
	char* program_name = basename(program_path);
	printf("Frontend made by Clownacy\n\n  Usage:  %s [mode] [inputdir] [outputdir]\n\n  Modes:\n   kc - Kosinski (compress)\n   kd - Kosinski (decompress)\n   ec - Enigma (compress)\n   ed - Enigma (decompress)\n   nc - Nemesis (compress)\n   nd - Nemesis (decompress)\n   sc - Saxman (compress)\n   sd - Saxman (decompress)\n", program_name);
}

bool checkFileExist(const char* const file_path)
{
	struct stat buffer;
	bool file_exists = (stat(file_path, &buffer) == 0);
	if (!file_exists)
	{
		char* filename = (char*)malloc(strlen(file_path)+1);	// The '+1' is for the null-character
		strcpy(filename, file_path);
		filename = basename(filename);
		printf("\n  File '%s' not found\n", filename);
		free(filename);
	}
	return file_exists;
}

bool handleModuleError(const bool success, const char* const module_filename)
{
	if (success == false)
	{
		printf("\n  Error initialising %s\n", module_filename);
		checkFileExist(module_filename);	// Check if our error is the module file being missing
	}
	return success;
}

bool initKosinskiModule(void)
{
	return handleModuleError(KInit(kosinski_path), kosinski_path);
}

bool initEnigmaModule(void)
{
	return handleModuleError(EInit(enigma_path), enigma_path);
}

bool initNemesisModule(void)
{
	return handleModuleError(NInit(nemesis_path), nemesis_path);
}

bool initSaxmanModule(void)
{
	return handleModuleError(SInit(saxman_path), saxman_path);
}

void processFile(void)
{
	if (strcmp(settings, "kc") == 0)
		if (initKosinskiModule() == true)
			KComp(input_path, output_path, false);
	else if (strcmp(settings, "kd") == 0)
		if (initKosinskiModule() == true)
			KDecomp(input_path, output_path, 0, false);
	else if (strcmp(settings, "ec") == 0)
		if (initEnigmaModule() == true)
			EComp(input_path, output_path, false);
	else if (strcmp(settings, "ed") == 0)
		if (initEnigmaModule() == true)
			EDecomp(input_path, output_path, 0, false);
	else if (strcmp(settings, "nc") == 0)
		if (initNemesisModule() == true)
			NComp(input_path, output_path);
	else if (strcmp(settings, "nd") == 0)
		if (initNemesisModule() == true)
			NDecomp(input_path, output_path, 0);
	else if (strcmp(settings, "sc") == 0)
		if (initSaxmanModule() == true)
			SComp(input_path, output_path, false);
	else if (strcmp(settings, "sd") == 0)
		if (initSaxmanModule() == true)
			SDecomp(input_path, output_path, 0, 0);
	else
		usage();
}

int main(int argc, char* argv[])
{
	program_path = argv[0];
	settings = argv[1];
	input_path = argv[2];
	output_path = argv[3];

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
		if(checkFileExist(input_path) == false)
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
