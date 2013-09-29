#ifndef CINTERFACE_H
#define CINTERFACE_H

#include <map>
#include <string>

// TODO 1 : At least validate the input
// TODO 2 : Change it to a GUI
class CommandLineInterface
{
private:
	std::map<int, std::string> m_DemoChoices;
	std::map<int, std::string> m_GameChoices;
	std::map<int, std::string> m_TestChoices;

public:
	CommandLineInterface();
	~CommandLineInterface();

	std::string ChooseROM();
};

#endif // CINTERFACE_H
