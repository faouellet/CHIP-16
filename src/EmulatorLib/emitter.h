#ifndef EMITTER_H
#define EMITTER_H

#include "memoryarena.h"
#include "utils.h"

/**
* \class Emitter
* \brief Emit x86 instructions to a memory buffer
* \author Felix-Antoine Ouellet
*/
class Emitter
{
private:
	Utils::UInt8* m_BufferPtr;	/*!< Pointer to a buffer of executable memory */
	MemoryArena m_Arena;		/*!< Memory arena of executable code */

public:
	/**
	* \fn Emitter
	* \brief Default constructor
	*/
	Emitter();

	/**
	* \fn ~Emitter
	* \brief Destructor
	*/
	~Emitter();

public:	// Arithmetic instructions
};

#endif // EMITTER_H
