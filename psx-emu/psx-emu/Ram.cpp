#define _CRT_SECURE_NO_DEPRECATE 
#include "Ram.hpp"
#include <stdio.h> 
#include <string.h>

Ram::Ram(const char * bios)
{ 
	memset(memory, 0, RAM_SIZE);

	FILE * file = fopen(bios, "r");

	if (file)
	{
		fprintf(stdout, "Loading bios\n");
		unsigned int index = 0;
		while (feof(file) == false)
		{
			memory[index] = fgetc(file);
		}
		fprintf(stdout, "Loaded bios\n");

		fclose(file);
	}
}