#include "DebugUtils.hpp"
#define _CRT_SECURE_NO_DEPRECATE 
#include <stdio.h>

void DebugUtils::print_word_binary(const char * message, unsigned int value)
{
	fprintf(stdout, "%s %d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d\n",
		message,
		value >> 31 & 0x1,
		value >> 30 & 0x1,
		value >> 29 & 0x1,
		value >> 28 & 0x1,
		value >> 27 & 0x1,
		value >> 26 & 0x1,
		value >> 25 & 0x1,
		value >> 24 & 0x1,
		value >> 23 & 0x1,
		value >> 22 & 0x1,
		value >> 21 & 0x1,
		value >> 20 & 0x1,
		value >> 19 & 0x1,
		value >> 18 & 0x1,
		value >> 17 & 0x1,
		value >> 16 & 0x1,
		value >> 15 & 0x1,
		value >> 14 & 0x1,
		value >> 13 & 0x1,
		value >> 12 & 0x1,
		value >> 11 & 0x1,
		value >> 10 & 0x1,
		value >> 9 & 0x1,
		value >> 8 & 0x1,
		value >> 7 & 0x1,
		value >> 6 & 0x1,
		value >> 5 & 0x1,
		value >> 4 & 0x1,
		value >> 3 & 0x1,
		value >> 2 & 0x1,
		value >> 1 & 0x1,
		value & 0x1
	);
}

void DebugUtils::print_halfword_binary(const char * message, unsigned short value)
{
	fprintf(stdout, "%s %d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d\n",
		message,
		value >> 15 & 0x1,
		value >> 14 & 0x1,
		value >> 13 & 0x1,
		value >> 12 & 0x1,
		value >> 11 & 0x1,
		value >> 10 & 0x1,
		value >> 9 & 0x1,
		value >> 8 & 0x1,
		value >> 7 & 0x1,
		value >> 6 & 0x1,
		value >> 5 & 0x1,
		value >> 4 & 0x1,
		value >> 3 & 0x1,
		value >> 2 & 0x1,
		value >> 1 & 0x1,
		value & 0x1
	);
}

void DebugUtils::print_byte_binary(const char * message, unsigned char value)
{
	fprintf(stdout, "%s %d%d%d%d%d%d%d%d\n",
		message,
		value >> 7 & 0x1,
		value >> 6 & 0x1,
		value >> 5 & 0x1,
		value >> 4 & 0x1,
		value >> 3 & 0x1,
		value >> 2 & 0x1,
		value >> 1 & 0x1,
		value & 0x1
	);
}