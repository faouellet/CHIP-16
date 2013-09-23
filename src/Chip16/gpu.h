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
};

#endif // GPU_H
