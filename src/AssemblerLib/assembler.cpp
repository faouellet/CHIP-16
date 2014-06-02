#include "assembler.h"

#include <fstream>
#include <iostream>

Assembler::Assembler(): 
    m_DataRegex("\s*db #?([0-9]+|\"[A-Za-z0-9\s]+\")\s*;"), 
    m_InstructionRegex("\s*([a-z]+) ([#A-Za-z0-9_]+)?,? ([#A-Za-z0-9_]+)?,? ([#A-Za-z0-9_]+)?\s*;"), 
    m_LabelRegex("\s*:([A-Za-z0-9]+)\s*;"), 
    m_MacroRegex("importbin \s*;")
{
	m_ValidInstructions = { 
		"nop", "cls", "vblnk", "bgc", "spr", "drw", "rnd", "flip", 
		"snd0", "snd1", "snd2", "snd3", "snp", "sng",
		"jmp", "jme", "call", "ret"
		"jz", "jnz", "jn", "jnn", "jp", "jo", "jno", "ja", "jae", "jb", "jbe", "jg", "jge", "jl", "jle", "jc", "jnc",
		"cz", "cnz", "cn", "cnn", "cp", "co", "cno", "ca", "cae", "cb", "cbe", "cg", "cge", "cl", "cle", "cc", "cnc",
		"ldi", "ldm", "mov", "stm",
		"addi", "add", "subi", "sub", "cmpi", "cmp", "andi", "and", "tsti", "tst",
		"ori", "or", "xori", "xor", "muli", "mul", "divi", "div",
		"shl", "shr", "sal", "sar",
		"push", "pop", "pushall", "popall", "pushf", "popf", "pal"
	};
}

Assembler::~Assembler() 
{
}

bool Assembler::Assemble(const std::string & in_Filename)
{
	std::ifstream l_Stream(in_Filename);
    std::string l_Dump;
	std::vector<std::string> l_FileContents;

	if(l_Stream.is_open())
	{
		while(l_Stream.eof())
		{
            std::getline(l_Stream, l_Dump);
            l_FileContents.push_back(l_Dump);
		}
		
		l_Stream.close();
	}
	else
	{
		std::cout << "ERROR: Problem reading the given assembly file" << std::endl;
		return false;
	}

	if (!l_FileContents.empty())
	{
		if (FirstPass(l_FileContents))
			SecondPass(l_FileContents);
	}
	else
	{
		std::cout << "WARNING: The given assembly is empty" << std::endl;
	}
	
	return true;
}
	
bool Assembler::FirstPass(const std::vector<std::string> & in_FileContents)
{
	for(auto& l_Line : in_FileContents)
	{
        
	}
}

void Assembler::SecondPass(const std::vector<std::string> & in_FileContents) const
{
	for(auto& l_Line : in_FileContents)
	{
	}
}

