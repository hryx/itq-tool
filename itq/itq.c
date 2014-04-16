/* itq: Frontend program to convert .it to .itq and vice versa
 * by hryx 2014
 * Public Domain
 *
 * Usage:
 *   itq encode source.it
 *   itq decode source.itq
 */

#include <stdio.h>
#include <string.h>
#include "modplug.h"

#define USAGE \
"Usage:\n" \
"  itq encode source.it    compress .it file into .itq\n" \
"  itq decode source.itq   decompress .itq file into .it\n"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf(USAGE);
		return 1;
	}
	else if (memcmp(argv[1], "encode", 6))
	{
		printf("Encoding (fake)...\n");
	}
	else if (memcmp(argv[1], "decode", 6))
	{
		printf("Decoding (fake)...\n");
	}

	return 0;
}
