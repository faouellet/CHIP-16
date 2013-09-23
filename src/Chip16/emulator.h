#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>
#include <vector>

#include "cpu.h"
#include "gpu.h"
#include "spu.h"

/*
* \class Emulator
* \brief Emulate the Chip16 architecture
* \author Félix-Antoine Ouellet
* \version 0.1
*/
class Emulator
{
private:
	std::vector<const unsigned char> m_ROMData;		/*!< Data + header of a c16 file */

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
	* \fn LoadROM
	* \brief Read a ROM from disk and load it into the emulator
	* \param in_ROMName The absolute path to the ROM
	* \return Success or failure
	*/
	bool LoadROM(const std::string & in_ROMName);
};

#endif // EMULATOR_H
