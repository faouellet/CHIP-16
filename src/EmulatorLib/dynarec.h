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
	typedef void (Emitter::*EmitFunc) (const UInt16 in_Arg1, const UInt16 in_Arg2);

private:
	//std::map<> m_TranslationCache;	/*!< */

	LocalAllocator m_Allocator;
	std::shared_ptr<CPU> m_CPU;		/*!< The central processing unit */
	std::unordered_map<UInt16, EmitFunc> m_EmitTable;
	Emitter m_Emitter;				/*!< x86 code emitter */

public:
	/**
	* \fn Dynarec
	* \brief Default constructor
	*/
	Dynarec(const std::shared_ptr<CPU> & in_CPU = nullptr);
	
	/**
	* \fn ~Dynarec
	* \brief Destructor
	*/
	~Dynarec();

	/**
	* \fn CompileBasicBlock
	* \brief TODO
	*/
	void CompileBasicBlock();

	/**
	* \fn ExecuteBlock
	* \brief TODO
	*/
	UInt8* ExecuteBlock() const;
};

#endif // DYNAREC_H
