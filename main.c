// KENS v1.4 frontend
// Copyright © 2015-2018 Clownacy

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "KENS.h"

#ifdef __cplusplus
extern "C" {
#endif

static char *program_path;
static char *settings;
static char *input_path;
static char *output_path;

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

static void ProcessFile(void)
{
	if (!strcmp(settings, "kc"))
		KComp(input_path, output_path, 8192, 256, false);
	else if (!strcmp(settings, "kd"))
		KDecomp(input_path, output_path, 0, false);
	else if (!strcmp(settings, "ec"))
		EComp(input_path, output_path, false);
	else if (!strcmp(settings, "ed"))
		EDecomp(input_path, output_path, 0, false);
	else if (!strcmp(settings, "nc"))
		NComp(input_path, output_path);
	else if (!strcmp(settings, "nd"))
		NDecomp(input_path, output_path, 0);
	else if (!strcmp(settings, "sc"))
		SComp(input_path, output_path, false);
	else if (!strcmp(settings, "sd"))
		SDecomp(input_path, output_path, 0, 0);
	else
		PrintUsage();
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
