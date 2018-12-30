#pragma once

#define NUM_REG 32

class RegisterFile final
{
public:
	void reset();

	unsigned int get(unsigned index);
	void set(unsigned int index, unsigned int val);

private:
	unsigned int r[NUM_REG];
};