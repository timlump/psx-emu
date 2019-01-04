#include "Cpu.hpp"

#include "DebugUtils.hpp"

#include "Ram.hpp"
#include "RegisterFile.hpp"
#include "Coprocessor0.hpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define BIOS_START 0xbfc00000

enum opcode : unsigned char {
	op_addiu = 0b001001, 
	op_ori = 0b001101, 
	op_lb =  0b100000,
	op_lui = 0b001111,
	op_sw = 0b0101011,
	op_beq = 0b000100,
	op_bne = 0b000101,
	op_bgtz = 0b000111,
	op_j = 0b000010   
};

enum funcs : unsigned char
{
	op_add = 0b100000,
	op_addu = 0b100001,
	op_and = 0b100100,
	op_nor = 0b100111,
	op_or = 0b100101,
	op_xor = 0b100110,
	op_sub = 0b100010,
	op_subu = 0b100011,
	op_slt = 0b101010,
	op_sltu = 0b101011,
	op_sll = 0b000000,
	op_sllv = 0b000100,
	op_sra = 0b000011,
	op_srav = 0b000111,
	op_srl = 0b000010,
	op_srlv = 0b000110,
	op_div = 0b011010,
	op_divu = 0b011011,
	op_mfhi = 0b010000,
	op_mflo = 0b010010,
	op_mthi = 0b010001,
	op_mtlo = 0b010011,
	op_mult = 0b011000,
	op_multu = 0b011001
};

void Cpu::reset()
{
	pc = BIOS_START;
	lo = rand();
	hi = rand();
}

bool Cpu::run() 
{
	registers->apply_delayed_loads();
	decode_and_execute(fetch(), true);

	if (stepByStep)
	{
		getchar();
	}

	return true;
}

unsigned int Cpu::fetch()
{
	unsigned int instruction = 0x0;

	instruction |= ram->get(pc + 3);
	instruction <<= 8;

	instruction |= ram->get(pc + 2);
	instruction <<= 8;

	instruction |= ram->get(pc + 1);
	instruction <<= 8;

	instruction |= ram->get(pc);

	if (stepByStep)
	{
		fprintf(stdout, "pc: %x\n", pc);
		DebugUtils::print_word_binary("instruction: ", instruction);
	}

	return instruction;
}

void Cpu::decode_and_execute(unsigned int instruction, bool advance_pc)
{
	unsigned int saved_pc = pc;

	// nop
	if (instruction == 0x0)
	{
		if (advance_pc && stepByStep)
		{
			fprintf(stdout, "nop\n");
		}
		
		pc += 4;
		return;
	}

	unsigned char op = instruction >> 26;

	unsigned int delay_instruction = 0x0;

	if (op == 0x0)
	{
		if (stepByStep)
		{
			fprintf(stdout, "register format\n");
		}
		delay_instruction = register_execute(instruction);
	}
	else if (op >> 1 == 0x1)
	{
		if (stepByStep)
		{
			fprintf(stdout, "jump format\n");
		}
		delay_instruction = jump_execute(instruction);
	}
	else if (op >> 2 == 0x4)
	{
		if (stepByStep)
		{
			fprintf(stdout, "coprocessor format\n");
		}
		delay_instruction = coprocessor_execute(instruction );
	}
	else
	{
		if (stepByStep)
		{
			fprintf(stdout, "immediate format\n");
		}
		delay_instruction = immediate_execute(instruction);
	}

	if (advance_pc)
	{
		decode_and_execute(delay_instruction, false);
	}
	else
	{
		pc = saved_pc;
	}
	
}

unsigned int Cpu::register_execute(unsigned int instruction)
{
	unsigned int pc_offset = 4;

	unsigned char src1 = (instruction >> 21) & 0x1f;
	unsigned char src2 = (instruction >> 16) & 0x1f;
	unsigned char dst = (instruction >> 11) & 0x1f;
	unsigned char shift = (instruction >> 6) & 0x1f;
	unsigned char func = instruction & 0x3f;

	if (stepByStep)
	{
		fprintf(stdout, "register format\n");
		DebugUtils::print_byte_binary("src1: ", src1);
		DebugUtils::print_byte_binary("src2: ", src2);
		DebugUtils::print_byte_binary("dst: ", dst);
		DebugUtils::print_byte_binary("shift: ", shift);
		DebugUtils::print_byte_binary("func: ", func);
	}

	switch (func)
	{
	case funcs::op_and:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_and\n");
		}
		registers->set(dst, registers->get(src1) + registers->get(src2)); 
	}
	break;
	case funcs::op_nor:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_nor\n");
		}
		registers->set(dst, ~(registers->get(src1) | registers->get(src2)));
	}
	break;
	case funcs::op_or:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_or\n");
		}
		registers->set(dst, registers->get(src1) | registers->get(src2));
	}
	break;
	case funcs::op_xor:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_xor\n");
		}
		registers->set(dst, registers->get(src1) ^ registers->get(src2));
	}
	break;
	case funcs::op_addu:
	case funcs::op_add:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_add/addu\n");
		}
		registers->set(dst, registers->get(src1) + registers->get(src2));

		// todo detect overflow if add  
	}
	break;
	case funcs::op_subu:
	case funcs::op_sub:
	{

		if (stepByStep)
		{
			fprintf(stdout, "op_sub/subu\n");
		}

		registers->set(dst, registers->get(src1) - registers->get(src2));

		// todo handle underflow when unsigned
	}
	break;
	case funcs::op_sll:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_sll\n");
		}
		registers->set(dst, registers->get(src2) << shift);
	}
	break;
	case funcs::op_sllv:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_sllv\n");
		}
		registers->set(dst, registers->get(src2) << registers->get(src1));
	}
	break;
	case funcs::op_sra: 
	case funcs::op_srl:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_sra/srl\n");
		}

		unsigned int val = registers->get(src2);

		unsigned int result = val >> shift; 

		if (func == funcs::op_sra)
		{
			int sVal = (int)val;
			sVal >>= shift;
			val = (unsigned int)sVal;
		}
		else
		{
			val >>= shift;
		}

		registers->set(dst, val);
	}
	break;
	case funcs::op_srav:
	case funcs::op_srlv:
	{

		if (stepByStep)
		{
			fprintf(stdout, "op_srav/srlv\n");
		}

		unsigned int val = registers->get(src2);
		unsigned int shift = registers->get(src1);

		if (func == funcs::op_srav)
		{
			int sVal = (int)val;
			sVal >>= shift;
			val = (unsigned int)sVal;
		}
		else
		{
			val >>= shift;
		}

		registers->set(dst, val);
	}
	break;
	case funcs::op_slt:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_slt\n");
		}
		if ((int)(registers->get(src1)) < (int)(registers->get(src2)))
		{
			registers->set(dst, 1);
		}
		else
		{
			registers->set(dst, 0);
		}
	}
	break;
	case funcs::op_sltu:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_sltu\n");
		}
		if (registers->get(src1) < registers->get(src2) )
		{
			registers->set(dst, 1);
		}
		else
		{
			registers->set(dst, 0);
		}
	}
	break;
	case funcs::op_div:
	case funcs::op_divu:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_div/divu\n");
		}
		unsigned int rs = registers->get(src1); 
		unsigned int rd = registers->get(src2);

		if (func = funcs::op_div)
		{
			int sRs = (int)rs;
			int sRd = (int)rd;

			hi = int(sRs % sRd);
			lo = int(sRs / sRd);
		}
		else
		{
			hi = rs % rd;
			lo = rs / rd;
		}
	}
	break;
	case funcs::op_mfhi:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_mfhi\n");
		}
		registers->set(dst, hi);
	}
	break;
	case funcs::op_mflo:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_mflo\n");
		}
		registers->set(dst, lo);
	}
	break;
	case funcs::op_mthi:
	{
		hi = registers->get(src1);
	}
	break;
	case funcs::op_mtlo:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_mtlo\n");
		}
		lo  = registers->get(src1);
	}
	break;
	case funcs::op_mult:
	case funcs::op_multu:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_mult/multu\n");
		}
		hi = lo = registers->get(src1)*registers->get(src2);
	}
	break;
	default:
		fprintf(stderr, "unknown func\n");
		getchar();
		stepByStep = true;
		break;
	}

	pc += pc_offset;

	return 0;
}

unsigned int Cpu::jump_execute(unsigned int instruction)
{
	unsigned char op = instruction >> 26;
	unsigned int target = instruction & 0x03ffffff;

	if (stepByStep)
	{
		fprintf(stdout, "jump format\n");
		DebugUtils::print_byte_binary("opcode: ", op);
		DebugUtils::print_word_binary("target: ", target);
	}

	switch (op)
	{
	case opcode::op_j:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_j\n");
		}
		pc = (pc & 0xf0000000) | (target << 2);
	}
	break;
	default:
		fprintf(stderr, "unknown opcode\n");
		getchar();
		stepByStep = true;
		break;
	}

	return 0;
}

unsigned int Cpu::immediate_execute(unsigned int instruction)
{
	unsigned int pc_offset = 4;

	unsigned char op = instruction >> 26;
	unsigned char src = (instruction >> 21) & 0x1f;
	unsigned char dst = (instruction >> 16) & 0x1f;
	unsigned short imm = (instruction) & 0xffff;

	if (stepByStep)
	{
		fprintf(stdout, "immediate format\n");
		DebugUtils::print_byte_binary("opcode: ", op);
		DebugUtils::print_byte_binary("src: ", src);
		DebugUtils::print_byte_binary("dst: ", dst);
		DebugUtils::print_halfword_binary("imm: ", imm);

		fprintf(stdout, "source register: %d dest register: %d\n", src, dst);
	}

	switch (op)
	{
	case opcode::op_addiu:
	{ 
		if (stepByStep)
		{
			fprintf(stdout, "op_addiu\n");
		}
		registers->set(dst, registers->get(src) + imm);
	}
	break;
	case opcode::op_beq:
		if (stepByStep)
		{
			fprintf(stdout, "op_beq\n");
		}
		if (registers->get(src) == registers->get(dst))
		{
			int val = (short)imm;
			pc_offset = 0;
			pc += val << 2;
		}
		break;
	case opcode::op_bgtz:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_bgtz\n");
		}
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
		if (stepByStep)
		{
			fprintf(stdout, "op_bne\n");
		}
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
		if (stepByStep)
		{
			fprintf(stdout, "op_lui\n");
		}
		registers->set(dst, imm << 16, true);
	}
	break;
	case opcode::op_lb:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_lb\n");
		}
		int addr = (short)imm + (int)registers->get(src);
		unsigned char val = ram->get(addr);
		registers->set(dst, (int)val, true);
	}
	break; 
	case opcode::op_ori:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_ori\n");
		}
		registers->set(dst, registers->get(src) | imm);
	}
	break;
	case opcode::op_sw:
	{
		if (stepByStep)
		{
			fprintf(stdout, "op_sw\n");
		}
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
		stepByStep = true;
	}

	pc += pc_offset;

	return 0;
}

unsigned int Cpu::coprocessor_execute(unsigned int instruction)
{
	fprintf(stdout, "coprocessor format\n");
	unsigned char op = instruction >> 26;

	if (op == 0b010000)
	{
		co0->execute(instruction);
	}
	else if (op == 0b010010)
	{
		fprintf(stdout, "GTE\n");
	}
	else
	{
		assert(false);
	}

	pc += 4;

	return 0;
}

bool Cpu::test()
{
	bool result = true;


	return result;
}