#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>

#include "cpu.h"
#include "dynarec.h"
#include "interpreter.h"

using Utils::UInt8;
using Utils::UInt16;
using Utils::UInt32;

/**
* \class Emulator
* \brief Emulate the Chip16 architecture
* \author Felix-Antoine Ouellet
*/
class Emulator
{
private:
	/**
	* \enum
	* \brief Useful integral constants
	*/
	enum { CPU_SPEED = 1000000, FPS = 60, CYCLES_PER_FRAME = CPU_SPEED/FPS };

private:
	static const float FRAME_TIME;

private:
	CPU m_CPU;					/*!< Central processing unit state */
	Dynarec m_Dynarec;			/*!< Recompiler engine */
	Interpreter m_Interpreter;	/*!< Interpreter engine */

public:
	/**
	* \fn Emulator
	* \brief Default constructor
	*/
	Emulator();

	/**
	* \fn ~Emulator
	* \brief Destructor
	*/
	~Emulator();
	
public:
	/**
	* \fn Init
	* \brief Initialize the processing units after reading a ROM from disk
	* \param in_ROMName The absolute path to the ROM
	* \return Code specifying success or the type of error that happened
	*/
	unsigned Init(const std::string & in_ROMName);

	/**
	* \fn Emulate
	* \brief Emulate the Chip16 by interpreting opcodes one at the time
	* \param in_UseDynarec Run the emulator in mixed mode or not 
	*/
	void Emulate(bool in_UseMixedMode);

private:
	/**
	* \fn ReadROM
	* \brief Read a ROM from disk
	* \param in_ROMName The absolute path to the ROM
	* \return Success or failure
	*/
	std::vector<UInt8> ReadROM(const std::string & in_ROMName);

	/**
	* \fn Interpret
	* \brief Run the emulator in pure interpretive mode
	*/
	void Interpret();

	/**
	* \fn JIT
	* \brief Run the emulator using a a mix of interpretation and dynamic recompilation
	*/
	void JIT();
};

#endif // EMULATOR_H
