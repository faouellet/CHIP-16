#include "spu.h"

#include <functional>

const UInt8 SPU::m_AttackValues[16] = { 2, 8, 16, 24, 38, 56, 68, 80, 100, 250, 500, 800, 1000, 3000, 5000, 8000 };
const UInt8 SPU::m_DecayValues[16] = { 6, 24, 48, 72, 114, 168, 204, 240, 300, 750, 1500, 2400, 3000, 9000, 15000, 24000 };
const UInt8 SPU::m_ReleaseValues[16] = { 6, 24, 48, 72, 114, 168, 204, 240, 300, 750, 1500, 2400, 3000, 9000, 15000, 24000 };

SPU::SPU() : m_AudioPaused(false) { }

SPU::~SPU() 
{
	SDL_CloseAudio();
}

bool SPU::Init() 
{
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
		return Utils::SPUError;

	using namespace std::placeholders;

	//m_AudioGen.callback = std::bind(&SPU::AudioCallback, this, _1, _2, _3);
	m_AudioGen.channels = 1;
	m_AudioGen.format = AUDIO_S16SYS;
	//m_AudioGen.samples = buffer size
}

void SPU::Reset() 
{
	if(!m_AudioPaused)
		SDL_PauseAudio(1);
}

void SPU::GenerateSound(UInt32 in_Attack, UInt32 in_Decay, UInt32 in_Sustain, 
		UInt32 in_Release, UInt32 in_Volume, UInt32 in_Type) 
{
	// Stop audio to prepare new signal
	if(!m_AudioPaused)
		SDL_PauseAudio(1);
	
	// Q : Are they really useful ??
	m_Attack = in_Attack;
	m_Decay = in_Decay;
	m_Sustain = in_Sustain;
	m_Release = in_Release;
	m_Volume = in_Volume;
	m_Sustain = in_Sustain;

	// TODO
	switch (in_Type)
	{
		case Triangle:
		{
			break;
		}
		case Sawtooth:
		{
			break;
		}
		case Pulse:
		{
			break;
		}
		case Noise:
		{
			break;
		}
		default:	// Panic
		{
			break;
		}
	}

	// Play
	SDL_PauseAudio(0);
}

void SPU::PlayTone(unsigned in_Tone, unsigned in_Duration) 
{
	// Stop audio to prepare new signal
	if(!m_AudioPaused)
		SDL_PauseAudio(1);
	
	// TODO
	
	// Play
	SDL_PauseAudio(0);
}

void SPU::Stop() 
{
	if(!m_AudioPaused)
		SDL_PauseAudio(1); 
}

void SPU::AudioCallback(void* in_UserData, Uint8* in_Stream, int in_Length)
{

}

int16_t SPU::GenerateSample()
{

}
