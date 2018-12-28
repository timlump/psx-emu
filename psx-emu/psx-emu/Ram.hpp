#pragma once
#include "DebugUtils.hpp"

// ref: https://problemkaputt.de/psx-spx.htm#memorymap  
#define MEMORY_SIZE 512*1024*1024
#define BIOS_START 0xbfc00000

class Ram
{
public:
	Ram(const char * bios_file);

	unsigned char& operator[](unsigned int addr);

private:
	unsigned char memory[MEMORY_SIZE];
};