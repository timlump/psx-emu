#pragma once

#define NUM_REG 32

#include "Ram.hpp"

// ref: https://www.scss.tcd.ie/~jones/vivio/dlx/dlxtutorial.htm
// ref: https://problemkaputt.de/psx-spx.htm
// ref: https://vhouten.home.xs4all.nl/mipsel/r3000-isa.html
// ref: http://alumni.cs.ucr.edu/~vladimir/cs161/mips.html

class Cpu final
{
public:
	void run();

	Ram * ram = nullptr; 

private:

	unsigned int pc;
	unsigned int r[NUM_REG];

	// the 5 stages of the mips pipeline
	void fetch();
	void decode();
	void execute();
	void memAccess();
	void writeBack();
};