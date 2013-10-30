#include "emulator.h"

#include <fstream>

const float Emulator::FRAME_TIME = (1.f/60.f)*1000.f;

Emulator::Emulator() { }

Emulator::~Emulator() { }

unsigned Emulator::Init(const std::string & in_ROMName)
{
	auto l_ROMData = ReadROM(in_ROMName);
	if(!l_ROMData.empty())
		return m_CPU.Init(std::move(l_ROMData));
	else return FileError;
}

void Emulator::Emulate(bool in_UseMixedMode)
{
	if(in_UseMixedMode)
		JIT();
	else
		Interpret();
}

void Emulator::Interpret()
{
	// TODO : Handle errors at this level

	bool l_Continue = true;
	int l_NbCycles;

	SDL_Event l_ControllerEvent;
	Uint32 l_StartTime;

	while(l_Continue)
	{
		l_NbCycles = 0;
		l_StartTime = SDL_GetTicks();

		// Each instruction takes exactly 1 cycle to execute.
		// Therefore, the emulator can interpret a precise number of instructions per video frame
		while(l_NbCycles < CYCLES_PER_FRAME)
		{
			m_Interpreter.InterpretOp();
			++l_NbCycles;
		}

		// Handle IO
		while(SDL_PollEvent(&l_ControllerEvent) && 
			(l_ControllerEvent.type == SDL_KEYDOWN || l_ControllerEvent.type == SDL_KEYUP))
			m_CPU.UpdateController(l_ControllerEvent.key);

		// TODO : Sound, later...
		
		// Draw on screen
		m_Interpreter.Show();
		
		// Sleep
		Uint32 l_Delay = static_cast<Uint32>(std::ceil(FRAME_TIME - (SDL_GetTicks() - l_StartTime)));
		if(static_cast<int>(l_Delay) > 0)
			SDL_Delay(l_Delay);
	}
}

void Emulator::JIT()
{
	// TODO : Black magic
}

std::vector<UInt8> Emulator::ReadROM(const std::string & in_ROMName)
{
	std::fstream l_FileStream(in_ROMName, std::ios::in | std::ios::binary);
	std::vector<UInt8> l_ROMData;

	if(l_FileStream.is_open())
	{
		l_FileStream.seekg(0, std::ios::end);
		auto size = l_FileStream.tellg();
		l_FileStream.seekg(0, std::ios::beg);

		l_ROMData.resize(size);
		l_FileStream.read((char*)&l_ROMData[0], size);

		l_FileStream.close();
	}

	return l_ROMData;
}
