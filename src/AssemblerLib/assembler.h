#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <map>
#include <regex>
#include <set>
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
	std::regex m_CommentRegex;
	std::regex m_LabelRegex;
	std::regex m_InstructionRegex;
	std::regex m_DataRegex;
	std::regex m_MacroRegex;

	std::map<std::string, UInt16> m_SymbolTable;

	std::set<std::string> m_ValidInstructions;

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
	 * \brief Read the file to populate the symbols table and check for syntax errors
	 * \param in_FileContents The assembly contained in a file 
	 * \return Pass completed without error?
	 */
	bool FirstPass(const std::vector<std::string> & in_FileContents);

	/* 
	 * \fn SecondPass
	 * \brief Read the file to assemble a binary file
	 * \param in_FileContents The assembly contained in a file 
	 */
	void SecondPass(const std::vector<std::string> & in_FileContents) const;
};

#endif // ASSEMBLER_H

