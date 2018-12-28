#include "Cpu.hpp"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum opcode : unsigned char {
	op_lb =  0b100000,
	op_beq = 0b000100,
	op_lwl = 0b100010,
	op_llo = 0b011000,
	op_lui = 0b001111,
	op_bnel = 0b010101,    
	op_hlt = 0b111111
};

void Cpu::reset()
{
	pc = BIOS_START;
	lo = rand();
	hi = rand();

	for (int i = 0; i < NUM_REG; i++)
	{
		registers.set(i, rand());
	}
}

bool Cpu::run() 
{
	fetch();
	decode();
 	execute();
	memAccess();
	writeBack();

	if (getchar() == 'q')
	{
		return false;
	}

	return true;
}

void Cpu::fetch()
{
	fprintf(stdout, "pc: %x\n", pc);
	DebugUtils::print_mem_map(pc);

	instruction = 0x0;

	instruction |= (*ram)[pc];
	instruction <<= 8;

	instruction |= (*ram)[pc + 1];
	instruction <<= 8;

	instruction |= (*ram)[pc + 2];
	instruction <<= 8;

	instruction |= (*ram)[pc + 3];

	DebugUtils::print_word_binary("instruction: ", instruction);
}

void Cpu::decode()
{
	unsigned char op  = instruction >> 26;

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
}

void Cpu::execute()
{
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
		default:
			fprintf(stderr, "unknown opcode\n");
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
		default:
			fprintf(stderr, "unknown func\n");
			break;
		}
	}
	else if (instruction_format == instruction_format::coprocessor_format)
	{
		fprintf(stdout, "coprocessor format\n");
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
		case opcode::op_beq:
			fprintf(stdout, "op_beq\n");
			if (registers.r[src] == registers.r[dst])
			{
				int val = (short)imm;
				pc_offset = 0; 
				pc += val << 2; 
			}
		break;
		case opcode::op_lui:
		{
			fprintf(stdout, "op_lui\n");
			registers.set(dst, imm << 16);
		}
		break;
		case opcode::op_lb:
		{
			fprintf(stdout, "op_lb\n");
			int addr = (short)imm + (int)registers.r[src];
			unsigned char val = (*ram)[addr];
			registers.set(dst, (int)val);
		}
		break;
		case opcode::op_lwl:
		{
			fprintf(stdout, "op_lwl\n");
			int index = (short)imm + (int)registers.r[src];
			unsigned int val = 0x0;

			val |= (*ram)[index];
			val <<= 8;

			val |= (*ram)[index+1];
			val <<= 8;

			val |= (*ram)[index+2];
			val <<= 8;

			val |= (*ram)[index+3];

			registers.set(dst, val);
		}
		break;
		case opcode::op_llo:
		{
			fprintf(stdout, "op_llo\n");
			unsigned int val = registers.r[dst];
			val &= 0xffff0000;
			val |= imm;
		}
		break;
		default:
			fprintf(stderr, "unknown opcode\n");
		}
	}

	pc += pc_offset;
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