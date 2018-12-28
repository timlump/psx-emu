#define _CRT_SECURE_NO_DEPRECATE 
#include "Ram.hpp"
#include <stdio.h> 
#include <string.h>

enum MemSeg
{
	KUSEG = 0x0,
	KSEG0 = 0x80000000,
	KSEG1 = 0xA0000000,
	KSEG2 = 0xC0000000
};

Ram::Ram(const char * bios_file)
{ 
	FILE * file = fopen(bios_file, "rb");

	if (file)
	{
		unsigned int index = 0;
		while (feof(file) == false)
		{
			unsigned char val = fgetc(file);
			(*this)[BIOS_START+index] = val;
			index++;
		} 
		fclose(file);
	}
}

unsigned char& Ram::operator[](unsigned int addr)
{ 
	addr &= 0x0fffffff;
	return memory[addr];
}