#define _CRT_SECURE_NO_DEPRECATE 
#include "Ram.hpp"
#include <stdio.h> 
#include <string.h>

Ram::Ram(const char * bios_file)
{ 
	FILE * file = fopen(bios_file, "rb");

	if (file)
	{
		unsigned int index = 0;
		while (feof(file) == false)
		{
			unsigned char val = fgetc(file);
			set(BIOS_START+index,val);
			index++;
		} 
		fclose(file);
	}
}

unsigned char Ram::get(unsigned int addr)
{
	addr &= 0x0fffffff;
	return memory[addr]; 
}

void Ram::set(unsigned int addr, unsigned char val)
{
	addr &= 0x0fffffff;
	memory[addr] = val;
}