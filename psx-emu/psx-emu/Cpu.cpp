#include "Cpu.hpp"
#include <stdio.h>
#include <assert.h>

enum opcode : unsigned char {
	// alu instructions
	op_add =  0b100000,
	op_addu = 0b100001,
	op_addi = 0b001000,
	op_addiu =0b001001,
	op_and =  0b100100,
	op_andi = 0b001100,
	op_div =  0b011010,
	op_divu = 0b011011,
	op_mult = 0b011000,
	op_multu =0b011001,
	op_nor =  0b100111,
	op_or =   0b100101,
	op_ori =  0b001101,
	op_sll =  0b000000,
	op_sllv = 0b000100,
	op_sra =  0b000011,
	op_srav = 0b000111,
	op_srl =  0b000010,
	op_srlv = 0b000110,
	op_sub =  0b100010,
	op_subu = 0b100011,
	op_xor =  0b100110,
	op_xori = 0b001110,
	// constant manipulating instructions
	op_lhi = 0b011001,
	op_llo = 0b011000,
	op_slt = 0b101010,
	op_sltu = 0b101001,
	op_slti = 0b001010,
	op_sltiu = 0b001001,
	// branch instructions
	op_beq = 0b000100,
	op_bgtz = 0b000111,
	op_blez = 0b000110,
	op_bne = 0b000101,
	// jump instructions
	op_j = 0b000010,
	op_jal = 0b000011,
	op_jalr = 0b001001,
	op_jr = 0b001000,
	// load instructions
	op_lb = 0b100000,
	op_lbu = 0b100100,
	op_lh = 0b100001,
	op_lhu = 0b100101,
	op_lw = 0b100011,
	// store instructions
	op_sb = 0b101000,
	op_sh = 0b101001,
	op_sw = 0b101011,
	// data movement instructions
	mfhi = 0b010000,
	mflo = 0b010010,
	mthi = 0b010001,
	mtlo = 0b010011,
	// exception and interrupt instructions
	trap = 0b011010
};

void Cpu::run() 
{
	fetch();
	decode();
 	execute();
	memAccess();
	writeBack();

	getchar();
}

void Cpu::fetch()
{
	fprintf(stdout, "pc: %d\n", pc);
	b0 = ram->raw[pc];
	b1 = ram->raw[pc + 1];
	b2 = ram->raw[pc + 2];
	b3 = ram->raw[pc + 3];
}

void Cpu::decode()
{
	unsigned int instruction = b0 << 26 | b1 << 16 | b2 << 8 | b3;
	operand.func = b0 >> 2;

	fprintf(stdout, "instruction %u \n", instruction);

	// first 6 bits identify the instruction format
	switch (operand.func & 0b111110)
	{
	case instruction_format::register_format:
		operand.func = instruction & 0b00111111;
		operand.shift = instruction >> 6 & 0b00011111;
		operand.dest = instruction >> 11 & 0b00011111;
		operand.src2 = instruction >> 16 & 0b00011111;
		operand.src1 = instruction >> 21 & 0b00011111;
		operand.instruction_format = instruction_format::register_format; 
		fprintf(stdout, "register format - func: %d\n", operand.func );
		break;

	case instruction_format::jump_format:
		operand.offset = instruction & 0x03ffffff;
		operand.instruction_format = instruction_format::jump_format; 
		fprintf(stdout, "jump format offset: %d\n", operand.offset); 
		break;

	case instruction_format::coprocessor_format:
		operand.func = instruction & 0b00111111;
		operand.dest = instruction >> 6 & 0b00011111;
		operand.src2 = instruction >> 11 & 0b00011111;
		operand.src1 = instruction >> 16 & 0b00011111;
		operand.format = instruction >> 21 & 0b00011111;
		operand.instruction_format = instruction_format::coprocessor_format;
		fprintf(stdout, "coprocessor format\n");
		break;

	default:
		operand.immediate = instruction;
		operand.dest = instruction >> 16 & 0b00011111;
		operand.src1 = instruction >> 21 & 0b00011111;
		operand.instruction_format = instruction_format::immediate_format;
		fprintf(stdout, "immediate format\n");
		break;
	} 

	// todo - don't forget to modify offset with jumps
	pc += 4;
}

void Cpu::execute()
{
	// jump opcodes overlap the logical shift opcodes so
	// we special case it
	if (operand.instruction_format == instruction_format::jump_format)
	{
		switch (operand.func)
		{
		case opcode::op_j:
			pc = (pc & 0xf0000000) | (operand.offset << 2);
			fprintf(stdout, "op_j\n");
			break;
		case opcode::op_jal:
			pc = (pc & 0xf0000000) | (operand.offset << 2);
			registers.set(31, pc + 4);
			fprintf(stdout, "op_jal\n");
			break;
		case opcode::op_jalr:
			pc = operand.src1;
			registers.set(31, pc + 4);
			fprintf(stdout, "op_jalr\n");
			break;
		case opcode::op_jr:
			pc = operand.src1;
			fprintf(stdout, "op_jr\n");
			break;
		}
	}
	else
	{
		switch (operand.func)
		{
		case opcode::op_add:
			break;
		}
	}
}

void Cpu::memAccess()
{

}

void Cpu::writeBack()
{

}

void Cpu::add()
{

}