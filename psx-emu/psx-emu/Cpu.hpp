#pragma once

#define NUM_REG 32

#include "Ram.hpp"

// ref: https://www.scss.tcd.ie/~jones/vivio/dlx/dlxtutorial.htm
// ref: https://problemkaputt.de/psx-spx.htm
// ref: https://vhouten.home.xs4all.nl/mipsel/r3000-isa.html
// ref: http://alumni.cs.ucr.edu/~vladimir/cs161/mips.html
// ref: https://cgi.cse.unsw.edu.au/~cs3231/doc/R3000.pdf
// ref: http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch05s07.html
// ref: http://www.mrc.uidaho.edu/mrc/people/jff/digital/MIPSir.html

enum instruction_format : unsigned char {
	register_format = 0x0, // 000000 (6) src1 (5) src2 (5) dest (5) shift (5) function (6)
	jump_format = 0b000010, // 00001x (6) target offset (26) 
	coprocessor_format = 0b010000, // 0100xx (6) format (5) src1 (5) src2 (5) dest (5) function (6)
	immediate_format = 0b11111 // opcode (6) source (5) dest (5) immediate (16)
};

class Cpu final
{
public:
	void run();

	Ram * ram = nullptr; 

private:

	unsigned int pc;
	 
	unsigned char b0;
	unsigned char b1;
	unsigned char b2;
	unsigned char b3;

	unsigned int r[NUM_REG];

	struct
	{
		unsigned int r[NUM_REG];
		void set(int index, unsigned int value)
		{
			if (index != 0)
			{
				r[index] = value; 
			}
		}
	} registers;

	struct
	{
		unsigned char src1;
		unsigned char src2;
		unsigned char dest;
		unsigned char shift;
		unsigned char format;
		unsigned char func;
		unsigned short immediate;
		unsigned int offset;
		instruction_format instruction_format;
	} operand;

	// the 5 stages of the mips pipeline
	void fetch();
	void decode();
	void execute();
	void memAccess();
	void writeBack();

	// operations
	void add();
};