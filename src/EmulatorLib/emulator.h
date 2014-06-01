#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.h"
#include "interpreter.h"

#include <string>

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
	std::shared_ptr<CPU> m_CPU;		/*!< Central processing unit state */
	Interpreter m_Interpreter;		/*!< Interpreter engine */

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
	*/
	void Emulate();

private:
	/**
	* \fn ReadROM
	* \brief Read a ROM from disk
	* \param in_ROMName The absolute path to the ROM
	* \return Success or failure
	*/
	std::vector<UInt8> ReadROM(const std::string & in_ROMName);
};

#endif // EMULATOR_H
