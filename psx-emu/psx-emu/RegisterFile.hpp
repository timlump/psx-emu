#pragma once

#include <tuple>

#define NUM_REG 32

class RegisterFile final
{
public:
	void reset();

	unsigned int get(unsigned index);
	void set(unsigned int index, unsigned int val, bool delay = false);

	void apply_delayed_loads();

private:

	unsigned int r[NUM_REG];
	
};