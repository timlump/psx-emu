#define _CRT_SECURE_NO_DEPRECATE 
#include "Ram.hpp"
#include <stdio.h> 
#include <string.h>

Ram::Ram(const char * bios)
{ 
	memset(raw, 0, RAM_SIZE);

	FILE * file = fopen(bios, "rb");

	if (file)
	{
		unsigned int index = 0;
		while (feof(file) == false)
		{
			raw[index] = fgetc(file);
			index++;
		}
		fprintf(stdout, "Loaded bios - pc end: %d \n", index-1);

		fclose(file);
	}
}