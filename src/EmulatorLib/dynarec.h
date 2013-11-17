#ifndef DYNAREC_H
#define DYNAREC_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "localallocator.h"
#include "cpu.h"
#include "emitter.h"

/**
* \class Dynarec
* \brief Dynamically recompile Chip16 instructions to x86 instructions
* \author Felix-Antoine Ouellet
*/
class Dynarec
{
private:
	/**
	* \enum
	* \brief x86 registers
	*/
	enum { 
		EAX = 0,
		ECX = 1,
		EDX = 2,
		EBX = 3,
		ESP = 4,
		EBP = 5,
		ESI = 6,
		EDI = 7 
	};

private:
	typedef void (Emitter::*EmitFunc) (const UInt16 in_Arg1, const UInt16 in_Arg2);	/**< Assembly emission function */

private:
	std::map<UInt8> m_TranslationCache;	/*!< */

	LocalAllocator m_Allocator;		/*!< Register allocator operating at the level of a single basic block */
	std::shared_ptr<CPU> m_CPU;		/*!< The central processing unit */
	std::unordered_map<UInt16, EmitFunc> m_EmitTable;	/*!< Mapping of Chip16 opcodes to assembly emission methods */
	Emitter m_Emitter;				/*!< x86 code emitter */

public:
	/**
	* \fn Dynarec
	* \brief Default constructor
	* \param in_CPU Pointer to an abstraction of a central processing unit
	*/
	Dynarec(const std::shared_ptr<CPU> & in_CPU = nullptr);
	
	/**
	* \fn ~Dynarec
	* \brief Destructor
	*/
	~Dynarec();

	/**
	* \fn CompileBasicBlock
	* \brief Recompile a Chip16 basic block to an x86 basic block
	*/
	void CompileBasicBlock();

	/**
	* \fn ExecuteBlock
	* \brief Execute a block of x86 assembly

	*/
	void ExecuteBlock() const;
};

#endif // DYNAREC_H
