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

#endif // CONSTANTS_H

