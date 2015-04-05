/* itq: Frontend program to convert .it to .itq and vice versa
 * by hryx 2014-2015
 * Public Domain
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modplug.h"

#define USAGE \
"Usage:\n" \
"  itq -h                           print this help message\n" \
"  itq enc [options] {infile}       compress .it file into .itq\n" \
"  itq dec [options] {infile}       decompress .itq file into .it\n" \
"Options:\n" \
"  -o outfile                       set output filename\n" \
"  -q quality                       vorbis quality (-0.1 to 1.0)\n"

typedef enum {
	CMD_NONE = 0,
	CMD_OUTPUT_IT,
	CMD_OUTPUT_ITQ
} command;

typedef enum {
	OPT_NONE = 0,
	OPT_SET_OUTNAME,
	OPT_SET_QUALITY
} option;

typedef enum {
	ERR_OK = 0,
	ERR_NOTOK,
	ERR_NOTYET,
	ERR_BAD_CMD,
	ERR_NO_INFILE,
	ERR_INFILE_PROBLEM,
	ERR_OUTFILE_PROBLEM,
	ERR_MEMFAIL,
	ERR_FILE_READ,
	ERR_MODPLUG
} error;

int main(int argc, char** argv)
{
	int i = 0;
	int err = 0;
	char* arg = 0;
	command cmd = CMD_NONE;
	option opt = OPT_NONE;
	char* infilename = 0;
	char* outfilename = 0;
	FILE* infile = 0;
	char* inbuffer = 0;
	long int insize = 0;
	long int bytesread = 0;
	float quality = 0.2;
	ModPlugFile* modfile = 0;

	/**** Process command line arguments ****/

	/* Ensure at least subcommand or -h */
	if (argc < 2) {
		err = ERR_BAD_CMD;
		goto pass_away;
	}

	/* Subcommand */
	arg = argv[1];
	if (strcmp(arg, "-h") == 0) {
		printf(USAGE);
		goto pass_away;
	}
	else if (strcmp(arg, "enc") == 0) {
		cmd = CMD_OUTPUT_ITQ;
	}
	else if (strcmp(arg, "dec") == 0) {
		cmd = CMD_OUTPUT_IT;
	}
	else {
		err = ERR_BAD_CMD;
		goto pass_away;
	}

	/* Options */
	for (i = 2; i < argc; i++) {
		char* arg = argv[i];
		if (opt == OPT_SET_OUTNAME) {
			outfilename = arg;
			opt = OPT_NONE;
		}
		else if (opt == OPT_SET_QUALITY) {
			quality = atof(arg);
			opt = OPT_NONE;
		}
		else if (strcmp(arg, "-o") == 0) {
			opt = OPT_SET_OUTNAME;
		}
		else if (strcmp(arg, "-q") == 0) {
			opt = OPT_SET_QUALITY;
		}
		else {
			infilename = arg;
		}
	}

	/* Make sure we have the necessities */
	if (!infilename) {
		err = ERR_NO_INFILE;
		goto pass_away;
	}
	if (!outfilename) {
		if (cmd == CMD_OUTPUT_IT) {
			outfilename = "out.it";
		}
		else if (cmd == CMD_OUTPUT_ITQ) {
			outfilename = "out.itq";
		}
		else { /* This shouldn't happen */
			err = ERR_NOTOK;
			goto pass_away;
		}
	}

	/* Make options reasonable */
	if (quality < -0.1 || quality > 1.0) {
		if (quality < -0.1) {
			quality = -0.1;
		}
		else if (quality > 1.0) {
			quality = 1.0;
		}
		printf("Note: Clamping quality setting to %.2f\n", quality);
	}

	/**** Load the mod ****/

	/* Read the source file */
	infile = fopen(infilename, "rb");
	if (!infile) {
		err = ERR_INFILE_PROBLEM;
		goto pass_away;
	}
	fseek(infile, 0, SEEK_END);
	insize = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	inbuffer = malloc(insize);
	if (!inbuffer) {
		err = ERR_MEMFAIL;
		goto pass_away;
	}
	bytesread = fread(inbuffer, 1, insize, infile);
	if (bytesread != insize) {
		err = ERR_FILE_READ;
		goto pass_away;
	}

	/* Turn source file into ModPlugFile */
	modfile = ModPlug_Load(inbuffer, insize);
	if (!modfile) {
		err = ERR_MODPLUG;
		goto pass_away;
	}

	/**** Process the file ****/

	/* Encode or decode and save new file */
	if (cmd == CMD_OUTPUT_ITQ) {
		printf("Encoding file...\n\n");
		if (!ModPlug_ExportITQ(modfile, outfilename, quality)) {
			err = ERR_OUTFILE_PROBLEM;
			goto pass_away;
		}
		printf("\nFile written to %s with quality setting %.2f\n", outfilename, quality);
	}
	else if (cmd == CMD_OUTPUT_IT) {
		err = ERR_NOTYET;
		goto pass_away;
		/*
		printf("Decoding file...\n");
		if (!ModPlug_ExportIT(modfile, outfilename)) {
			err = ERR_OUTFILE_PROBLEM;
			goto pass_away;
		}
		printf("\nFile written to %s\n", outfilename);
		*/
	}
	else { /* Should not happen */
		err = ERR_NOTOK;
		goto pass_away;
	}

	/**** Error emporium ****/

	pass_away:
	switch (err) {
		case ERR_NOTOK:
			printf("Error: Something went wrong and I have no idea what!\n");
			break;
		case ERR_NOTYET:
			printf("That feature is not yet implemented. Please accept my apologies.\n");
			break;
		case ERR_BAD_CMD:
			printf("Error: Bad command\n" USAGE);
			break;
		case ERR_NO_INFILE:
			printf("Error: No input file specified\n" USAGE);
			break;
		case ERR_INFILE_PROBLEM:
			printf("Error: Could not open source file %s\n", infilename);
			break;
		case ERR_OUTFILE_PROBLEM:
			printf("Error: Problem writing output file %s\n", outfilename);
			break;
		case ERR_MEMFAIL:
			printf("Error: Could not allocate memory for mod file\n");
			break;
		case ERR_FILE_READ:
			printf("Error: Could not read entire file into memory\n");
			printf("File size determined as %ld, read %ld\n", insize, bytesread);
			break;
		case ERR_MODPLUG:
			printf("Error: ModPlug could not load %s -- invalid mod file?\n", infilename);
			break;
		default:
			break;
	}
	return err;
}

#ifdef __cplusplus
}
#endif
