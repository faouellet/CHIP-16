#include "spu.h"

#include "SDL.h"	// TODO : Use SDL_Mixer for sound

SPU::SPU() { }

SPU::~SPU() { }

bool SPU::Init() { /* Call SDL_Mixer */ return true; }

void SPU::GenerateSound(unsigned long && in_OpCode) { }

void SPU::PlayTone(unsigned in_Tone, unsigned in_Duration) { }

void SPU::Stop() { }
