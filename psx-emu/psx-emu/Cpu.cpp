#include "Cpu.hpp"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum opcode : unsigned char {
	op_addiu = 0b001001, 
	op_ori = 0b001101, 
	op_lb =  0b100000,
	op_lwl = 0b100010,
	op_llo = 0b011000,
	op_lui = 0b001111,
	op_sw = 0b0101011,
	op_beq = 0b000100,
	op_bne = 0b000101,
	op_bgtz = 0b000111,
	op_j = 0b000010   
};

enum funcs : unsigned char
{
	op_sll = 0b000000,
	op_or = 0b100101 
};

void Cpu::reset()
{
	pc = BIOS_START;
	lo = rand();
	hi = rand();
}

bool Cpu::run() 
{
	decode_and_execute(fetch());

	return true;
}

unsigned int Cpu::fetch()
{
	fprintf(stdout, "pc: %x\n", pc);

	unsigned int instruction = 0x0;

	instruction |= ram->get(pc + 3);
	instruction <<= 8;

	instruction |= ram->get(pc + 2);
	instruction <<= 8;

	instruction |= ram->get(pc + 1);
	instruction <<= 8;

	instruction |= ram->get(pc);

	DebugUtils::print_word_binary("instruction: ", instruction);

	return instruction;
}

void Cpu::decode_and_execute(unsigned int instruction)
{
	unsigned char op = instruction >> 26;

	instruction_format instruction_format = instruction_format::register_format;

	if (op == 0x0)
	{
		instruction_format = instruction_format::register_format;
	}
	else if (op >> 1 == 0x1)
	{
		instruction_format = instruction_format::jump_format;
	}
	else if (op >> 2 == 0x4)
	{
		instruction_format = instruction_format::coprocessor_format;
	}
	else
	{
		instruction_format = instruction_format::immediate_format;
	}

	unsigned int pc_offset = 4;

	// nop
	if (instruction == 0x0)
	{
		fprintf(stdout, "nop\n");
		pc += pc_offset;
		return;
	}

	if (instruction_format == instruction_format::jump_format)
	{
		unsigned char op = instruction >> 26;
		unsigned int target = instruction & 0x03ffffff;

		fprintf(stdout, "jump format\n");
		DebugUtils::print_byte_binary("opcode: ", op);
		DebugUtils::print_word_binary("target: ", target);

		switch (op)
		{ 
		case opcode::op_j:
		{
			fprintf(stdout, "op_j\n");
			pc_offset = 0;
			pc = (pc & 0xf0000000) | (target << 2);
		}
		break;
		default:
			fprintf(stderr, "unknown opcode\n");
			getchar();
			break;
		}
	}
	else if (instruction_format == instruction_format::register_format)
	{
		unsigned char src1 = (instruction >> 21) & 0x1f;
		unsigned char src2 = (instruction >> 16) & 0x1f;
		unsigned char dst = (instruction >> 11) & 0x1f;
		unsigned char shift = (instruction >> 6) & 0x1f;
		unsigned char func = instruction & 0x3f;

		fprintf(stdout, "register format\n");
		DebugUtils::print_byte_binary("src1: ", src1);
		DebugUtils::print_byte_binary("src2: ", src2);
		DebugUtils::print_byte_binary("dst: ", dst);
		DebugUtils::print_byte_binary("shift: ", shift);
		DebugUtils::print_byte_binary("func: ", func);

		switch (func)
		{
		case funcs::op_sll:
		{
			fprintf(stdout, "op_sll\n");
			registers->set(dst, registers->get(src2) << shift);
		}
		break;
		case funcs::op_or:
		{  
			registers->set(dst, registers->get(src1) | registers->get(src2));
		}
		break; 
		default:
			fprintf(stderr, "unknown func\n");
			getchar();
			break;
		}
	}
	else if (instruction_format == instruction_format::coprocessor_format)
	{
		fprintf(stdout, "coprocessor format\n");
		getchar();
	}
	else if (instruction_format == instruction_format::immediate_format)
	{
		unsigned char op = instruction >> 26;
		unsigned char src = (instruction >> 21) & 0x1f;
		unsigned char dst = (instruction >> 16) & 0x1f;
		unsigned short imm = (instruction) & 0xffff;

		fprintf(stdout, "immediate format\n");
		DebugUtils::print_byte_binary("opcode: ", op);
		DebugUtils::print_byte_binary("src: ", src);
		DebugUtils::print_byte_binary("dst: ", dst);
		DebugUtils::print_halfword_binary("imm: ", imm);

		fprintf(stdout, "source register: %d dest register: %d\n", src, dst);

		switch (op)
		{
		case opcode::op_addiu:
		{ 
			fprintf(stdout, "op_addiu\n");
			registers->set(dst, registers->get(src)+imm);
		}
		break;
		case opcode::op_beq:
			fprintf(stdout, "op_beq\n");
			if (registers->get(src) == registers->get(dst))
			{
				int val = (short)imm;
				pc_offset = 0; 
				pc += val << 2; 
			}
		break;
		case opcode::op_bgtz:
		{
			fprintf(stdout, "op_bgtz\n");
			if (registers->get(src) > 0)
			{
				int val = (short)imm;
				pc_offset = 0;
				pc += val << 2;
			}
		}
		break;
		case opcode::op_bne:
		{
			fprintf(stdout, "op_bne\n");
			if (registers->get(src) != registers->get(dst))
			{
				int val = (short)imm;
				pc_offset = 0;
				pc += val << 2;
			}
		}
		break;
		case opcode::op_lui:
		{
			fprintf(stdout, "op_lui\n");
			registers->set(dst, imm << 16);
		}
		break;
		case opcode::op_lb:
		{
			fprintf(stdout, "op_lb\n");
			int addr = (short)imm + (int)registers->get(src);
			unsigned char val = ram->get(addr);
			registers->set(dst, (int)val);
		}
		break;
		case opcode::op_lwl:
		{
			fprintf(stdout, "op_lwl\n");
			int index = (short)imm + (int)registers->get(src);
			unsigned int val = 0x0;

			val |= ram->get(index+3);
			val <<= 8;

			val |= ram->get(index+2);
			val <<= 8;

			val |= ram->get(index+1);
			val <<= 8;

			val |= ram->get(index);

			registers->set(dst, val);
		}
		break;
		case opcode::op_ori:
		{
			fprintf(stdout, "op_ori\n");
			registers->set(dst, registers->get(src) | imm);
		}
		break;
		case opcode::op_llo:
		{
			fprintf(stdout, "op_llo\n");
			unsigned int val = registers->get(dst);
			val &= 0xffff0000;
			val |= imm;
		}
		break;
		case opcode::op_sw:
		{
			fprintf(stdout, "op_sw\n");
			int index = (registers->get(src) + (short)imm);
			unsigned int val = registers->get(dst);
			ram->set(index + 3, val >> 26);
			ram->set(index + 2, val >> 16);
			ram->set(index + 1, val >> 8);
			ram->set(index, val);
		}
		break;
		default:
			fprintf(stderr, "unknown opcode\n");
			getchar();
		}
	}

	pc += pc_offset;
}