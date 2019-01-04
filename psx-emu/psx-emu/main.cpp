#include <stdio.h> 
#include "Cpu.hpp"
#include "Ram.hpp"
#include "RegisterFile.hpp"
#include "Coprocessor0.hpp"

int main()
{  
	Cpu * cpu = new Cpu();
	cpu->reset();

	Ram * ram = new Ram();
	cpu->ram = ram;

	RegisterFile * registerFile = new RegisterFile();
	registerFile->reset();
	cpu->registers = registerFile;

	Coprocessor0 * co0 = new Coprocessor0();
	co0->cpu = cpu;
	cpu->co0 = co0;

	bool testPassed = cpu->test();

	if (testPassed)
	{
		ram->loadBios("F:/roms/SCPH5502.bin");

		while (true)
		{
			if (cpu->run() == false)
			{
				break;
			}
		} 
	}
	else
	{
		fprintf(stderr, "failed tests\n");
		getchar();
	}

	delete co0;
	delete registerFile;
	delete ram;
	delete cpu;

	return 0;
}