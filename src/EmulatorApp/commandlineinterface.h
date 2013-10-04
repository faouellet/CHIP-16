#ifndef CINTERFACE_H
#define CINTERFACE_H

#include <string>
#include <vector>

// TODO 1 : At least validate the input
// TODO 2 : Change it to a GUI

/**
* \class CommandLineInterface
* \brief Basic interface from which to choose a ROM
* \author Felix-Antoine Ouellet
*/
class CommandLineInterface
{
private:
	std::vector<std::string> m_DemoChoices;	/*!< Names of the demo ROMs  */
	std::vector<std::string> m_GameChoices;	/*!< Names of the game ROMs  */
	std::vector<std::string> m_TestChoices;	/*!< Names of the test ROMs  */

public:
	/**
	* \fn CommandLineInterface
	* \brief Default constructor
	*/
	CommandLineInterface();

	/**
	* \fn ~CommandLineInterface
	* \brief Destructor
	*/
	~CommandLineInterface();

	/**
	* \fn ChooseROM
	* \brief Ask the user to choose a demo, game or test ROM
	* \return The relative path to the chosen ROM
	*/
	std::string ChooseROM();

private:
	/**
	* \fn Choose
	* \brief Outputs ROM choices to the console and get the user's choice
	* \param in_Choices Possible ROMs to choose from
	* \return Index of the chosen ROM
	*/
	int Choose(const std::vector<std::string> in_Choices) const;

	/**
	* \fn FormatChoice
	* \brief Format the relative path to the chosen ROM
	* \param in_Folder Folder containing the chosen ROM
	* \param in_Choice Name of the chosen ROM
	* \return The formatted relative path to the chosen ROM
	*/
	std::string FormatChoice(const std::string & in_Folder, const std::string & in_Choice) const;
};

#endif // CINTERFACE_H
