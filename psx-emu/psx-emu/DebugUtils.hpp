#pragma once

class DebugUtils final
{
public:
	static void print_word_binary(const char * message, unsigned int value);
	static void print_halfword_binary(const char * message, unsigned short value);
	static void print_byte_binary(const char * message, unsigned char value);
	static void print_mem_map(unsigned int addr);
};