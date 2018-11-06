// KENS v1.4 frontend
// Copyright © 2015-2018 Clownacy

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "KENS/Include/KENS.h"

static char *program_path;
static char *settings;
static char *input_path;
static char *output_path;

static const char kosinski_path[] = "libs/Kosinski.dll";
static const char enigma_path[] = "libs/Enigma.dll";
static const char nemesis_path[] = "libs/Nemesis.dll";
static const char saxman_path[] = "libs/Saxman.dll";

static void PrintUsage(void)
{
	printf("Frontend made by Clownacy\n\n  Usage:  %s [mode] [inputdir] [outputdir]\n\n  Modes:\n   kc - Kosinski (compress)\n   kd - Kosinski (decompress)\n   ec - Enigma (compress)\n   ed - Enigma (decompress)\n   nc - Nemesis (compress)\n   nd - Nemesis (decompress)\n   sc - Saxman (compress)\n   sd - Saxman (decompress)\n", program_path);
}

static bool CheckFileExist(const char* const file_path)
{
	struct stat buffer;
	bool file_exists = !stat(file_path, &buffer);

	if (!file_exists)
		printf("\n  File '%s' not found\n", file_path);

	return file_exists;
}

static bool HandleModuleError(const bool success, const char* const module_filename)
{
	if (success == false)
	{
		printf("\n  Error initialising %s\n", module_filename);
		CheckFileExist(module_filename);	// Check if our error is the module file being missing
	}

	return success;
}

static bool InitKosinskiModule(void)
{
	return HandleModuleError(KInit(kosinski_path), kosinski_path);
}

static bool InitEnigmaModule(void)
{
	return HandleModuleError(EInit(enigma_path), enigma_path);
}

static bool InitNemesisModule(void)
{
	return HandleModuleError(NInit(nemesis_path), nemesis_path);
}

static bool InitSaxmanModule(void)
{
	return HandleModuleError(SInit(saxman_path), saxman_path);
}

static void ProcessFile(void)
{
	if (!strcmp(settings, "kc"))
	{
		if (InitKosinskiModule())
			KComp(input_path, output_path, false);
	}
	else if (!strcmp(settings, "kd"))
	{
		if (InitKosinskiModule())
			KDecomp(input_path, output_path, 0, false);
	}
	else if (!strcmp(settings, "ec"))
	{
		if (InitEnigmaModule())
			EComp(input_path, output_path, false);
	}
	else if (!strcmp(settings, "ed"))
	{
		if (InitEnigmaModule())
			EDecomp(input_path, output_path, 0, false);
	}
	else if (!strcmp(settings, "nc"))
	{
		if (InitNemesisModule())
			NComp(input_path, output_path);
	}
	else if (!strcmp(settings, "nd"))
	{
		if (InitNemesisModule())
			NDecomp(input_path, output_path, 0);
	}
	else if (!strcmp(settings, "sc"))
	{
		if (InitSaxmanModule())
			SComp(input_path, output_path, false);
	}
	else if (!strcmp(settings, "sd"))
	{
		if (InitSaxmanModule())
			SDecomp(input_path, output_path, 0, 0);
	}
	else
	{
		PrintUsage();
	}
}

int main(int argc, char *argv[])
{
	program_path = argv[0];
	settings = argv[1];
	input_path = argv[2];
	output_path = argv[3];

	switch (argc)
	{
	default:
		PrintUsage();
		break;

	#ifdef _WIN32
	// If opened by Explorer, don't close immediately
	case 1:
		PrintUsage();
		getchar();
		break;
	#endif

	case 4:
		if (CheckFileExist(input_path))
			ProcessFile();

		break;
	}

	// Done
	return 0;
}

#ifdef __cplusplus
}
#endif
