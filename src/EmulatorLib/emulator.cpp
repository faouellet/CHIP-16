#include "emulator.h"

#include <chrono>
#include <fstream>

Emulator::Emulator() { }

Emulator::~Emulator() { }

void Emulator::Emulate()
{
	// TODO1 : Handle errors at this level
	// TODO2 : Make sure to maintain 60 FPS

	bool l_Continue = true;
	int l_NbCycles;

	SDL_Event l_Controller1Event;
	//SDL_Event l_Controller2Event;

	while(l_Continue)
	{
		l_NbCycles = 0;
		
		// Each instruction takes exactly 1 cycle to execute.
		// Therefore, the emulator can interpret a precise number of instructions per video frame
		while(l_NbCycles < CYCLES_PER_FRAME)
		{
			m_CPU.InterpretOp();
			++l_NbCycles;
		}

		// Handle IO
		while(SDL_PollEvent(&l_Controller1Event) && 
			(l_Controller1Event.type == SDL_KEYDOWN || l_Controller1Event.type == SDL_KEYUP))
			m_CPU.UpdateController(1, l_Controller1Event.key);

		// Sound ???

		// Draw on screen
		m_CPU.FlushGPU();
	}
}

unsigned Emulator::Init(const std::string & in_ROMName)
{
	auto l_ROMData = ReadROM(in_ROMName);
	if(!l_ROMData.empty())
		return m_CPU.Init(std::move(l_ROMData));
	else return FileError;
}

std::vector<UInt8> Emulator::ReadROM(const std::string & in_ROMName)
{
	std::fstream l_FileStream(in_ROMName, std::ios::in | std::ios::binary);
	std::vector<UInt8> l_ROMData;

	if(l_FileStream.is_open())
	{
		// TODO : Find what kind of wizardry prevents the following from working
		/*l_ROMData.insert(l_ROMData.begin(), 
			std::istream_iterator<unsigned char>(l_FileStream), 
			std::istream_iterator<unsigned char>());*/
			
		l_FileStream.seekg(0, std::ios::end);
		int size = l_FileStream.tellg();
		l_FileStream.seekg(0, std::ios::beg);

		l_ROMData.resize(size);
		l_FileStream.read((char*)&l_ROMData[0], size);

		l_FileStream.close();
	}

	return l_ROMData;
}
