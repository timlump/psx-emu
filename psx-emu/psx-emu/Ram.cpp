#define _CRT_SECURE_NO_DEPRECATE 
#include "Ram.hpp"
#include <stdio.h> 
#include <string.h>

Ram::Ram(const char * bios)
{ 
	memset(raw, 0, RAM_SIZE);

	FILE * file = fopen(bios, "r");

	if (file)
	{
		fprintf(stdout, "Loading bios\n");
		unsigned int index = 0;
		while (feof(file) == false)
		{
			raw[index] = fgetc(file);
		}
		fprintf(stdout, "Loaded bios\n");

		fclose(file);
	}
}