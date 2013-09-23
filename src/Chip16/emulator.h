#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>

#include "cpu.h"
#include "gpu.h"
#include "spu.h"

#include "utils.h"

using Utils::UInt8;
using Utils::UInt16;
using Utils::UInt32;

/*
* \class Emulator
* \brief Emulate the Chip16 architecture
* \author Félix-Antoine Ouellet
* \version 0.1
*/
class Emulator
{
private:
	CPU m_CPU;	/*!< The central processing unit */
	GPU m_GPU;	/*!< The graphics processing unit */
	SPU m_SPU;	/*!< The sound processing unit */

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
	* \enum
	* \brief Masks for the possible errors that can happen during the initialization
	*/
	enum { NoError = 0, CPUError = 1, GPUError = 2, SPUError = 4, FileError = 8 };

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
	*/
	void Emulate();

private:
	/**
	* \fn ReadROM
	* \brief Read a ROM from disk
	* \param in_ROMName The absolute path to the ROM
	* \return Success or failure
	*/
	std::vector<const UInt8> ReadROM(const std::string & in_ROMName);
};

#endif // EMULATOR_H
