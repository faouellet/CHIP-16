#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <map>
#include <regex>
#include <string>
#include <vector>

#include "emitter.h"

/**
 * \class Assembler
 * \brief TODO
 */
class Assembler
{
private:
	std::regex m_LabelRegex;
	std::regex m_InstructionRegex;
	std::regex m_DataRegex;
	std::regex m_MacroRegex;

	std::map<std::string, UInt8> m_SymbolTable;

public:	
	/*
	 * \fn Assembler
	 * \brief Default constructor
	 */
	Assembler();
	
	/*
	 * \fn ~Assembler
	 * \brief Destructor
	 */
	~Assembler();

	/*
	 * \fn Assemble
	 * \brief Assemble a binary file from a Chip16 file
	 * \param in_Filename Name of the Chip16 assembly file
	 * \return Assembly succesful?
	 */
	bool Assemble(const std::string & in_Filename); 

private: // TODO: maybe add a ref to asm book
	/*
	 * \fn FirstPass
	 * \brief Read the file to populate the symbols table
	 * \param in_FileContents The assembly contained in a file 
	 */
	void FirstPass(const std::vector<std::string> & in_FileContents);

	/* 
	 * \fn SecondPass
	 * \brief Read the file to assemble a binary file
	 * \param in_FileContents The assembly contained in a file 
	 */
	void SecondPass(const std::vector<std::string> & in_FileContents) const;
};

#endif // ASSEMBLER_H

