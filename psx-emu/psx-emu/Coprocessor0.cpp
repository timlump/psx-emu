#include "Coprocessor0.hpp"
#include "Cpu.hpp"

// 0100xx (6) format (5) src1 (5) src2 (5) dest (5) function (6)
void Coprocessor0::execute(unsigned int instruction)
{
	unsigned char format = (instruction >> 21) & 0b11111;
	unsigned char src1 = (instruction >> 16) & 0b11111;
	unsigned char src2 = (instruction >> 11) & 0b11111;
	unsigned char dest = (instruction >> 6) & 0b11111;
	unsigned char func = instruction & 0b111111;


}