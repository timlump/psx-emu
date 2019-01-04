#define _CRT_SECURE_NO_DEPRECATE 
#include "Ram.hpp"
#include <stdio.h> 
#include <string.h>

#define BIOS_START 0xbfc00000

void Ram::loadBios(const char * bios_file)
{ 
	FILE * file = fopen(bios_file, "rb");

	if (file)
	{
		unsigned int index = 0;
		while (feof(file) == false)
		{
			unsigned char val = fgetc(file);
			unsigned int addr = (BIOS_START + index) & 0x0fffffff;
			memory[addr] = val;
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