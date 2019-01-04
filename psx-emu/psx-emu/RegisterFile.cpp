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

void RegisterFile::set(unsigned int index, unsigned int val, bool delay)
{ 
	if (index != 0 && delay == false)
	{
		r[index] = val;
	}

	if (index != 0 && delay == true)
	{
		std::tuple<int, unsigned int, unsigned int> load;
		std::get<0>(load) = 1;
		std::get<1>(load) = index;
		std::get<2>(load) = val;
	}
}

void RegisterFile::apply_delayed_loads()
{
	 
}