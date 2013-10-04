#ifndef GPU_TESTS_H
#define GPU_TESTS_H

#include "gpu.h"

/**
* \struct GPUFixture
* \brief Fixture containing data to test the GPU specific instructions.
*        That means the draw, flip and palette instructions.
* \author Felix-Antoine Ouellet
*/
struct GPUFixture
{
	GPU gpu;

	GPUFixture() { }
};

#endif // GPU_TESTS_H
