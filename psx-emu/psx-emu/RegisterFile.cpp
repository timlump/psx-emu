#include "RegisterFile.hpp"
#include <stdlib.h>

void RegisterFile::reset()
{
	for (int i = 0; i < NUM_REG; i++)
	{
		r[i] = rand();
	}

	r[0] = 0;  
}

unsigned int RegisterFile::get(unsigned index)
{
	return r[index];
}

void RegisterFile::set(unsigned int index, unsigned int val)
{
	if (index != 0)
	{
		r[index] = val;
	}
}