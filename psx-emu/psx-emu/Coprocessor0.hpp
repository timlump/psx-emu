#pragma once

class Cpu;

class Coprocessor0 final
{
public:
	void execute(unsigned int instruction);

	Cpu * cpu;
};