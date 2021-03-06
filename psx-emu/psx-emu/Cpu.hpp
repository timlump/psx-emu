#pragma once

// ref: https://www.scss.tcd.ie/~jones/vivio/dlx/dlxtutorial.htm
// ref: https://problemkaputt.de/psx-spx.htm
// ref: https://vhouten.home.xs4all.nl/mipsel/r3000-isa.html
// ref: http://alumni.cs.ucr.edu/~vladimir/cs161/mips.html
// ref: https://cgi.cse.unsw.edu.au/~cs3231/doc/R3000.pdf
// ref: http://www.cs.uwm.edu/classes/cs315/Bacon/Lecture/HTML/ch05s07.html
// ref *: http://www.mrc.uidaho.edu/mrc/people/jff/digital/MIPSir.html
// ref *: https://opencores.org/project/plasma/opcodes
// ref: https://eclass.teicrete.gr/modules/document/file.php/TP286/%CE%92%CE%BF%CE%B7%CE%B8%CE%B7%CF%84%CE%B9%CE%BA%CE%AC%20%CE%B5%CE%B3%CF%87%CE%B5%CE%B9%CF%81%CE%AF%CE%B4%CE%B9%CE%B1/MIPS_Instruction_Coding_With_Hex.pdf 
// ref: https://www.d.umn.edu/~gshute/mips/itype.xhtml   

enum instruction_format : unsigned char {
	register_format = 0x0, // 000000 (6) src1 (5) src2 (5) dest (5) shift (5) function (6)
	jump_format = 0b000010, // 00001x (6) target offset (26) 
	coprocessor_format = 0b010000, // 0100xx (6) format (5) src1 (5) src2 (5) dest (5) function (6)
	immediate_format = 0b11111 // opcode (6) source (5) dest (5) immediate (16)
};

class Ram;
class RegisterFile;
class Coprocessor0;

class DebugUtils final
{
public:
	static void print_word_binary(const char * message, unsigned int value);
	static void print_halfword_binary(const char * message, unsigned short value);
	static void print_byte_binary(const char * message, unsigned char value);
};

class Cpu final
{
public:
	void reset();
	bool run();
	bool test();

	Ram * ram = nullptr;
	RegisterFile * registers = nullptr;
	Coprocessor0 * co0 = nullptr;

	bool stepByStep = true;

private:

	unsigned int pc;
	unsigned int lo;
	unsigned int hi; 

	// the mips processor has a 5 step pipeline but I am not attempted a low level emulation but rather a high level one
	unsigned int fetch();
	void decode_and_execute(unsigned int instruction, bool advance_pc);

	unsigned int register_execute(unsigned int instruction);
	unsigned int jump_execute(unsigned int instruction);
	unsigned int immediate_execute(unsigned int instruction);
	unsigned int coprocessor_execute(unsigned int instruction);
};