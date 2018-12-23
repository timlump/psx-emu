#include "Cpu.hpp"
#include <stdio.h>

enum opcode : unsigned char {
	// alu instructions
	op_add = 0b100000,
	op_addu = 0b100001,
	op_addi = 0b001000,
	op_addiu = 0b001001,
	op_and = 0b100100,
	op_andi = 0b001100,
	op_div = 0b011010,
	op_divu = 0b011011,
	op_mult = 0b011000,
	op_multu = 0b011001,
	op_nor = 0b100111,
	op_or = 0b100101,
	op_ori = 0b100101,
	op_sll = 0b000000,
	op_sllv = 0b000100,
	op_sra = 0b000011,
	op_srav = 0b000111,
	op_srl = 0b000010,
	op_srlv = 0b000110,
	op_sub = 0b100010,
	op_subu = 0b100011,
	op_xor = 0b100110,
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

enum instruction_format : unsigned char {
	register_format = 0b000000, // 000000 (6) src1 (5) src2 (5) dest (5) shift (5) function (6)
	jump_format = 0b000100, // 0001xx (6) target offset (26) 
	coprocessor_format = 0b010000, // 0100xx (6) format (5) src1 (5) src2 (5) dest (5) function (6)
	immediate_format = 0b111111 // opcode (6) source (5) dest (5) immediate (16)
};

void Cpu::run()
{
	fetch();
	decode();
	execute();
	memAccess();
	writeBack();
}

void Cpu::fetch()
{
	b0 = ram->raw[pc];
	b1 = ram->raw[pc + 1];
	b2 = ram->raw[pc + 2];
	b3 = ram->raw[pc + 3];
}

void Cpu::decode()
{
	unsigned char opcode = b0 >> 2;

	unsigned int instruction = b0 << 26 | b1 << 16 | b2 << 8 | b3;

	switch (opcode & 0b111100)
	{
	case instruction_format::register_format:
		operand_function = instruction & 0b00111111;
		operand_shift = instruction >> 6 & 0b00011111;
		operand_dest = instruction >> 11 & 0b00011111;
		operand_src2 = instruction >> 16 & 0b00011111;
		operand_src1 = instruction >> 21 & 0b00011111;
		break;

	case instruction_format::jump_format:
		operand_offset = instruction & 0x03ffffff; 
		break;

	case instruction_format::coprocessor_format:
		operand_function = instruction & 0b00111111;
		operand_dest = instruction >> 6 & 0b00011111;
		operand_src2 = instruction >> 11 & 0b00011111;
		operand_src1 = instruction >> 16 & 0b00011111;
		operand_format = instruction >> 21 & 0b00011111;
		break;

	default:
		operand_immediate = instruction;
		operand_dest = instruction >> 16 & 0b00011111;
		operand_src1 = instruction >> 21 & 0b00011111;
		break;
	}

	switch (opcode)
	{
	default:
		fprintf(stderr, "unrecognised opcode\n");
	}
}

void Cpu::execute()
{

}

void Cpu::memAccess()
{

}

void Cpu::writeBack()
{

}