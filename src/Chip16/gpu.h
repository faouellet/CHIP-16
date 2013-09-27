#ifndef GPU_H
#define GPU_H

/*
* \class GPU
* \brief Emulates the Graphics Processing Unit (GPU)
* \author Félix-Antoine Ouellet
* \version 0.1
*/
class GPU
{
private:
	// TODO : ...
	// Most certainly a screen buffer

public:
	/**
	* \fn GPU
	* \brief Default constructor
	*/
	GPU();

	/**
	* \fn ~GPU
	* \brief Destructor
	*/
	~GPU();
	
public:
	/**
	* \fn Init
	* \brief Initialize the graphics processing unit
	* \return Success or failure
	*/
	bool Init();

	/**
	* \fn Draw
	* \brief Draw ...
	*/
	void Draw();
};

#endif // GPU_H
