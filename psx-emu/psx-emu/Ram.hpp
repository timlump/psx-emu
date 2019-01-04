#pragma once

// ref: https://problemkaputt.de/psx-spx.htm#memorymap  
#define MEMORY_SIZE 512*1024*1024

class Ram
{
public:
	void loadBios(const char * bios_file);

	unsigned char get(unsigned int addr);
	void set(unsigned int addr, unsigned char val);

private:

	unsigned char memory[MEMORY_SIZE];
	
};