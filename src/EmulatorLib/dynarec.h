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
		EDX = 2, 
		ECX = 1, 
		EDI = 7, 
		ESI = 6, 
		ESP = 4, 
		EBP = 5, 
		EBX = 3 
	};

private:
	//std::map<> m_TranslationCache;	/*!< */

	std::shared_ptr<CPU> m_CPU;
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

	// TODO : Analyse the content of an instruction

	void CompileBasicBlock();

	UInt8* ExecuteBlock() const;
};

#endif // DYNAREC_H
