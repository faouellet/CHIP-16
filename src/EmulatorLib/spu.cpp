#include "spu.h"

#include "SDL.h"	// TODO : Use SDL_Mixer for sound

SPU::SPU() { }

SPU::~SPU() { }

bool SPU::Init() 
{
	return SDL_Init(SDL_INIT_AUDIO) < 0;
}

void SPU::Reset() { }

void SPU::GenerateSound(UInt8 in_Attack, UInt8 in_Decay, UInt8 in_Sustain, 
		UInt8 in_Release, UInt8 in_Volume, UInt8 in_Type) { /*TODO*/ }

void SPU::PlayTone(unsigned in_Tone, unsigned in_Duration) { /*TODO*/ }

void SPU::Stop() { /*TODO*/ }
