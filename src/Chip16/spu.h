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
};

#endif // SPU_H
