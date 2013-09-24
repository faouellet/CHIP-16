#ifndef SPU_H
#define SPU_H

/*
* \class SPU
* \brief Emulates the Sound Processing Unit (SPU)
* \author Félix-Antoine Ouellet
* \version 0.1
*/
class SPU
{
private:
	// TODO : ...
	// Most certainly a current generator

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

public:
	/**
	* \fn Init
	* \brief Initialize the sound processing unit
	* \return Success or failure
	*/
	bool Init();
	
	/**
	* \fn GenerateSound
	* \brief Activate the sound generator
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void GenerateSound(unsigned long && in_OpCode);

	/**
	* \fn PlayTone
	* \brief Play a given tone for a given duration
	* \param in_Tone	 The frequency of the tone to play
	*        in_Duration The duration in milliseconds of the tone to play
	*/
	void PlayTone(unsigned in_Tone, unsigned in_Duration);

	/**
	* \fn Stop
	* \brief Stop the sound generator
	*/
	void Stop();
};

#endif // SPU_H
