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
	std::tuple<int, unsigned int, unsigned int> write;

	std::get<0>(write) = 2;
	std::get<1>(write) = addr;
	std::get<2>(write) = val;

	waiting_writes.push_back(write);
}

void Ram::write_back()
{
	if (waiting_writes.empty() == false)
	{
		for (auto iter : waiting_writes)
		{
			std::tuple<int, unsigned int, unsigned int> &write = iter;
			int val = std::get<0>(write);
			if (val == 0)
			{
				unsigned int addr = std::get<1>(write) & 0x0fffffff;
				memory[addr] = std::get<2>(write);
				waiting_writes.pop_front();
			}
			else
			{
				std::get<0>(write)--;
			}
		}
	}
}