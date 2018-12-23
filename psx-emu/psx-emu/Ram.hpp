#pragma once

#define RAM_SIZE 2048*1024

class Ram
{
public:
	Ram(const char * bios);
	unsigned char raw[RAM_SIZE];
};