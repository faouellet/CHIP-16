#ifndef DYNAREC_H
#define DYNAREC_H

#include "emitter.h"

/**
* \class Dynarec
* \brief Dynamically recompile Chip16 instructions to x86 instructions
*/
class Dynarec
{
private:
	Emitter m_Emitter;	/*!< x86 code emitter */

public:
	/**
	* \fn Dynarec
	* \brief Default constructor
	*/
	Dynarec();
	
	/**
	* \fn ~Dynarec
	* \brief Destructor
	*/
	~Dynarec();

	// ...
};

#endif // DYNAREC_H
