#pragma once

#define RAM_SIZE 2048*1024

class Ram
{
public:
	Ram(const char * bios);
private: 
	 unsigned char memory[RAM_SIZE];
};