/* itq: Frontend program to convert .it to .itq and vice versa
 * by hryx 2014
 * Public Domain
 *
 * Usage:
 *   itq encode source.it
 *   itq decode source.itq
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modplug.h"

#define USAGE \
"Usage:\n" \
"  itq encode source.it        compress .it file into .itq\n" \
"  itq decode source.itq       decompress .itq file into .it\n"

#define ARG_CMD argv[1]
#define ARG_FILE argv[2]
#define OUTPUT_IT 1
#define OUTPUT_ITQ 2

int main(int argc, char** argv)
{
	int action = 0;
	FILE* infile = 0;
	FILE* outfile = 0;
	char* inbuffer = 0;
	long int insize = 0;
	long int bytesread = 0;
	ModPlugFile* modfile = 0;

	/* Check command line arguments */
	if (argc != 3)
	{
		printf(USAGE);
		return 1;
	}
	if (memcmp(ARG_CMD, "encode", 6) == 0)
		action = OUTPUT_ITQ;
	else if (memcmp(ARG_CMD, "decode", 6) == 0)
		action = OUTPUT_IT;
	else
	{
		printf("Error: Incorrect usage\n");
		printf(USAGE);
		return 1;
	}

	/* Read the source file */
	infile = fopen(ARG_FILE, "rb");
	if (!infile)
	{
		printf("Error: Could not open source file %s\n", ARG_FILE);
		return 1;
	}
	fseek(infile, 0, SEEK_END);
	insize = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	inbuffer = malloc(insize);
	if (!inbuffer)
	{
		printf("Error: Could not allocate memory for mod file\n");
		return 1;
	}
	bytesread = fread(inbuffer, 1, insize, infile);
	if (bytesread != insize)
	{
		printf("Error: Could not read entire file into memory\n");
		printf("File size determined as %ld, read %ld\n", insize, bytesread);
		return 1;
	}

	/* Turn source file into ModPlugFile */
	modfile = ModPlug_Load(inbuffer, insize);
	if (!modfile)
	{
		printf("Error: ModPlug could not load %s -- invalid mod file?\n", ARG_FILE);
		return 1;
	}

	/* Encode or decode and save new file */
	if (action == OUTPUT_ITQ)
	{
		printf("Encoding to out.itq ...\n");
		if (!ModPlug_ExportITQ(modfile, "out.itq"))
		{
			printf("Error: Problem writing output file\n");
			return 1;
		}
	}
	else if (action == OUTPUT_IT)
	{
		printf("Decoding to out.it ...\n");
		if (!ModPlug_ExportIT(modfile, "out.it"))
		{
			printf("Error: Problem writing output file\n");
			return 1;
		}
	}
	else /* Should not happen */
	{
		printf("Something went wrong and I have no idea what!\n");
		return 1;
	}

	return 0;
}
