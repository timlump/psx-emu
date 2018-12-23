#include <stdio.h> 
#include "Cpu.hpp"
#include "Ram.hpp"

int main()
{  
	Cpu * cpu = new Cpu();
	Ram * ram = new Ram("F:/roms/SCPH5502.bin");
	cpu->ram = ram;

	while (true)
	{
		cpu->run();
	}

	return 0;
}