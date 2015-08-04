#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
* \enum
* \brief Masks for the possible errors that can happen during the initialization
*/
enum { NoError = 0, EmptyROMError = 1, ROMOverflowError = 2, GPUInitError = 4, 
		GPUWindowCreationError = 8, GPURendererError = 16, GPUTextureError = 32, 
		SPUError = 64, FileError = 128 };

/**
* \enum
* \brief Masks for the possible errors that can happen during the emulation
*/
enum { StackError = 1, UnknownOpError = 2, EmulationDone = 4 };

/**
* \enum
* \brief Useful size constants
* TODO: Should all of this be here?
*/
enum { HEADER_SIZE = 16, MEMORY_SIZE = 64 * 1024, STACK_START = 0xFDF0, CONTROLLER_1 = 0xFFF0, CONTROLLER_2 = 0xFFF2 };


#endif // CONSTANTS_H

