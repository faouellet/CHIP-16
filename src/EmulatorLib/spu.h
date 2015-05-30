#ifndef SPU_H
#define SPU_H

#include "../Utils/utils.h"

#include "SDL.h"

#include <functional>

using Utils::UInt8;
using Utils::UInt32;

/**
* \class SPU
* \brief Emulates the Sound Processing Unit (SPU)
*/
class SPU
{
private:
	/**
	* \struct AudioSample
	* \brief Characteristics of an audio sample
	*/
	struct AudioSample
	{
		UInt32 Attack;			/*!< Duration it takes to go from intensity 0 to max volume */
		UInt32 Decay;			/*!< Duration it takes to go from max intensity to sustain intensity */
		UInt32 Sustain;			/*!< Volume the sound will sustain after decay, until release */
		UInt32 Release;			/*!< Duration it takes to go from sustain intensity to intensity 0 */
		UInt32 Volume;			/*!< Peak volume of the sound */
		UInt32 Type;			/*!< Type of audio signal chosen between :
									 triangle wave, sawtooth wave, noise*/
	};

private:
	/**
	* \enum
	* \brief Type of sound wave
	*/
	enum { Triangle = 0, Sawtooth, Pulse, Noise };

private:
	bool m_AudioPaused;			/*!< Indicate whether the audio is paused or not. 
								     Necessary since SDL doesn't give a function to check it
									 and it is designed in a way that a call to SDL_PauseAudio
									 can actually resume audio playing */


	static const int m_AttackValues[16];		/*!< Possible attack values */
	static const int m_DecayValues[16];			/*!< Possible decay values */
	static const int m_ReleaseValues[16];		/*!< Possible release values */
	
	AudioSample m_Sample;		/*!< Characteristics of the current audio sample generated */

	static std::function<int16_t(void)> m_SampleGenerator;	/*!< Generate the current audio sample */

	SDL_AudioSpec m_AudioGen;	/*!< Generates the actual audio signal */

public:
	/**
	* \fn SPU
	* \brief Default constructor
	*/
	SPU();

	/**
	* \fn ~SPU
	* \brief Destructor
	*/
	~SPU();

	// TODO : Need a callback to fill the buffer in the SDL audio thread

public:
	/**
	* \fn Init
	* \brief Initialize the sound processing unit
	* \return Error code
	*/
	UInt8 Init();
	
	/**
	* \fn Reset
	* \brief Restore the sound processing unit at its pre-initialized state
	*/
	void Reset();

	/**
	* \fn GenerateSound
	* \brief Activate the sound generator
	* \param TODO
	*/
	void GenerateSound(UInt32 in_Attack, UInt32 in_Decay, UInt32 in_Sustain, 
		UInt32 in_Release, UInt32 in_Volume, UInt32 in_Type);

	/**
	* \fn PlayTone
	* \brief Play a given tone for a given duration
	* \param in_Tone The frequency in Hertz of the tone to play
	* \param in_Duration The duration in milliseconds of the tone to play
	*/
	void PlayTone(UInt8 in_Tone, unsigned in_Duration);

	/**
	* \fn Stop
	* \brief Stop the sound generator
	*/
	void Stop();

private:
	/**
	* \fn AudioCallback
	* TODO
	*/
	static void AudioCallback(void* in_UserData, Uint8* in_Stream, int in_Length);
	
	/**
	* \fn GenerateSoundSample
	* TODO
	*/
	static int16_t GenerateSoundSample();
	
	/**
	* \fn GenerateToneSample
	* TODO
	*/
	static int16_t GenerateToneSample();
};

#endif // SPU_H
