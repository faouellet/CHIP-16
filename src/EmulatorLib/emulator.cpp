#include "emulator.h"

#include "constants.h"

#include <fstream>

const float Emulator::FRAME_TIME = (1.f/60.f)*1000.f;

Emulator::Emulator(bool in_DynarecMode)
{
	// TODO: Do something with the dynarec
	//if (in_DynarecMode)
	//	m_Dynarec ...
	std::fill(std::begin(*m_Memory), std::end(*m_Memory), 0);
}

unsigned Emulator::Init(const std::string & in_ROMName)
{
	std::vector<UInt8> l_ROMData;
	unsigned l_ErrorCode = ReadROM(in_ROMName, l_ROMData);
	
	if(l_ErrorCode == NoError)
	{
		l_ErrorCode |= l_ROMData.empty() ? EmptyROMError : NoError;
		l_ErrorCode |= l_ROMData.size() > STACK_START ? ROMOverflowError : NoError;

		for (int i = 0; i < HEADER_SIZE; ++i)
			m_ROMHeader[i] = l_ROMData[i];

		for (unsigned i = HEADER_SIZE; i < l_ROMData.size(); ++i)
			(*m_Memory)[i - HEADER_SIZE] = l_ROMData[i];

		l_ErrorCode |= m_Interpreter.InitDevices();
	}

	return l_ErrorCode;
}

void Emulator::Emulate()
{
	// TODO : Handle errors at this level

	bool l_Continue = true;
	int l_NbCycles;

	
	Uint32 l_StartTime;
	
	Instruction l_Instruction;

	while(l_Continue)
	{
		l_NbCycles = 0;
		l_StartTime = SDL_GetTicks();

		// Each instruction takes exactly 1 cycle to execute.
		// Therefore, the emulator can interpret a precise number of instructions per video frame
		while(l_NbCycles < CYCLES_PER_FRAME)
		{
			if(m_Interpreter.InterpretOne() != NoError)
			{
				l_Continue = false;
				break;
			}
			++l_NbCycles;
		}

		// TODO: Should this be confined to the interpreter? What's gonna happen with the dynarec?
		m_Interpreter.HandleKeyboardInputs();

		// TODO : Sound, later...
		
		// Draw on screen
		m_Interpreter.Show();
		
		// Sleep
		Uint32 l_Delay = static_cast<Uint32>(std::ceil(FRAME_TIME - (SDL_GetTicks() - l_StartTime)));
		if(static_cast<int>(l_Delay) > 0)
			SDL_Delay(l_Delay);
	}
}

void Emulator::Reset()
{
	for (int i = 0; i < HEADER_SIZE; ++i)
		m_ROMHeader[i] = 0;

	std::fill(std::begin(*m_Memory), std::end(*m_Memory), 0);

	m_Interpreter.Reset();
}

unsigned Emulator::ReadROM(const std::string & in_ROMName, std::vector<UInt8> & out_ROMData)
{
	if (!out_ROMData.empty())
		out_ROMData.clear();

	std::fstream l_FileStream(in_ROMName, std::ios::in | std::ios::binary);
	
	if(l_FileStream.is_open())
	{
		l_FileStream.seekg(0, std::ios::end);
		auto size = l_FileStream.tellg();
		l_FileStream.seekg(0, std::ios::beg);

		out_ROMData.resize(size);
		l_FileStream.read((char*)&out_ROMData[0], size);

		l_FileStream.close();
		return NoError;
	}
	else
	{
		return FileError;
	}
}
