#pragma once

#define NUM_REG 32

#include "Ram.hpp"

// ref: https://www.scss.tcd.ie/~jones/vivio/dlx/dlxtutorial.htm
// ref: https://problemkaputt.de/psx-spx.htm
// ref: https://vhouten.home.xs4all.nl/mipsel/r3000-isa.html
// ref: http://alumni.cs.ucr.edu/~vladimir/cs161/mips.html
// ref: https://cgi.cse.unsw.edu.au/~cs3231/doc/R3000.pdf
// ref: http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch05s07.html

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

	unsigned char operand_src1;
	unsigned char operand_src2;
	unsigned char operand_dest;
	unsigned char operand_shift;
	unsigned char operand_format;
	unsigned char operand_function;
	unsigned short operand_immediate;
	unsigned int operand_offset;

	// the 5 stages of the mips pipeline
	void fetch();
	void decode();
	void execute();
	void memAccess();
	void writeBack();
};