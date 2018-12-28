#include <stdio.h> 
#include "Cpu.hpp"
#include "Ram.hpp"

int main()
{  
	Cpu * cpu = new Cpu();
	cpu->reset();

	Ram * ram = new Ram("F:/roms/SCPH5502.bin");
	cpu->ram = ram;

	while (true)
	{
		if (cpu->run() == false)
		{
			break;
		}
	}

	return 0;
}