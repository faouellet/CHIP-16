#ifndef CPU_H
#define CPU_H

/*
* \class CPU
* \brief Emulates the Central Processing Unit (SPU)
* \author Félix-Antoine Ouellet
* \version 0.1
*/
class CPU
{
private:
	unsigned short m_PC;		/*!< Program counter */
	unsigned short m_SP;		/*!< Stack pointer */
	unsigned char m_FR;			/*!< Flag register */
	unsigned short m_Regs[16];	/*!< General purpose registers */

public:
	/**
	* \fn CPU
	* \brief Default constructor
	*/
	CPU();

	/**
	* \fn ~CPU
	* \brief Destructor
	*/
	~CPU();

public:
	/**
	* \fn Init
	* \brief Initialize the central processing unit
	* \param in_PCStart The initial value of the program counter
	* \return Success or failure
	*/
	bool Init(const unsigned char in_PCStart);

	/**
	* \fn InterpretOp
	* \n brief Read an opcode from the ROM and execute it
	*/
	void InterpretOp();
};

#endif // SPU_H
