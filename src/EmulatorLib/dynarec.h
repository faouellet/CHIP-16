#ifndef DYNAREC_H
#define DYNAREC_H

#include <map>
#include <memory>

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
	//std::map<> m_TranslationCache;	/*!< */

	// NOTE : Could be interesting to have an abstraction over a basic block. This could help with 
	//        inter-block optimization if it happens. However, for the time being, the dynarec
	//        will only work over one basic block at the time.


	bool m_RegConstness[16];		/*!< Can the value of a register be modified within a basic block */
	std::shared_ptr<CPU> m_CPU;		/*!< The central processing unit */
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

private:
	/**
	* \fn AnalyzeRegisterLiveness
	* \brief TODO
	* TODO : Add ref to Dragon book
	*/
	std::vector<std::vector<bool>> AnalyzeRegisterLiveness(const std::vector<Instruction> & in_BasicBlock) const;
};

#endif // DYNAREC_H
