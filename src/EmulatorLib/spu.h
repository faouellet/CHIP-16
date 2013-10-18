#ifndef SPU_H
#define SPU_H

#include "SDL.h"

#include "utils.h"

using Utils::UInt8;
using Utils::UInt32;

/**
* \class SPU
* \brief Emulates the Sound Processing Unit (SPU)
* \author Felix-Antoine Ouellet
*/
class SPU
{
	// IDEA : Maybe a struct to encapsulate what a audio sample is supposed to be

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

	UInt32 m_Attack;			/*!< Duration it takes to go from intensity 0 to max volume */
	UInt32 m_Decay;				/*!< Duration it takes to go from max intensity to sustain intensity */
	UInt32 m_Sustain;			/*!< Volume the sound will sustain after decay, until release */
	UInt32 m_Release;			/*!< Duration it takes to go from sustain intensity to intensity 0 */
	UInt32 m_Volume;			/*!< Peak volume of the sound */
	UInt32 m_Type;				/*!< Type of audio signal chosen between :
								     triangle wave, sawtooth wave, noise*/

	static const UInt8 m_AttackValues[16];		/*!< Possible attack values */
	static const UInt8 m_DecayValues[16];		/*!< Possible decay values */
	static const UInt8 m_ReleaseValues[16];		/*!< Possible release values */

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
	* \return Success or failure
	*/
	bool Init();
	
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
	void PlayTone(unsigned in_Tone, unsigned in_Duration);

	/**
	* \fn Stop
	* \brief Stop the sound generator
	*/
	void Stop();

private:
	void AudioCallback(void* in_UserData, Uint8* in_Stream, int in_Length);
	int16_t GenerateSample();
};

#endif // SPU_H
