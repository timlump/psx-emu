#include <stdio.h> 
#include "Cpu.hpp"

int main()
{  
	Cpu * cpu = new Cpu();
	cpu->reset();

	Ram * ram = new Ram("F:/roms/SCPH5502.bin");
	cpu->ram = ram;

	RegisterFile * registerFile = new RegisterFile();
	registerFile->reset();
	cpu->registers = registerFile;

	while (true)
	{
		if (cpu->run() == false)
		{
			break;
		}
	}

	return 0;
}