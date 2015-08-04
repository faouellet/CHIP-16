#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.h"
#include "dynarec.h"
#include "interpreter.h"

#include <array>
#include <string>

using Utils::UInt8;
using Utils::UInt16;
using Utils::UInt32;

/**
* \class Emulator
* \brief Emulate the Chip16 architecture
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
	UInt8 m_ROMHeader[HEADER_SIZE];		/*!< The header of a .c16 file. See specs for details */
	std::shared_ptr<std::array<Uint8, MEMORY_SIZE>> m_Memory;	/*!< Memory of the CPU. See specs for layout details */

	Interpreter m_Interpreter;			/*!< Interpreter engine */
	Dynarec m_Dynarec;					/*!< Dynamic recompiler */

public:
	/**
	* \fn Emulator
	* \brief Default constructor
	* \param in_DynarecMode Is the emulator in dynarec mode or interpreter mode?
	*/
	Emulator(bool in_DynarecMode = false);

	/**
	* \fn ~Emulator
	* \brief Destructor
	*/
	~Emulator() = default;
	
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

	/**
	* \fn Reset
	* \brief Restore the emulator to its base state
	*/
	void Reset();

private:
	/**
	* \fn ReadROM
	* \brief Read a ROM from disk
	* \param in_ROMName  The absolute path to the ROM
	* \param out_ROMData Data contained in the ROM
	* \return Error code depending on the situation
	*/
	unsigned ReadROM(const std::string & in_ROMName, std::vector<UInt8> & out_ROMData);
};

#endif // EMULATOR_H
