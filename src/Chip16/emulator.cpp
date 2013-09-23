#include "emulator.h"

#include <fstream>

Emulator::Emulator() { }

Emulator::~Emulator() { }

void Emulator::Emulate()
{
	for(;;)
		m_CPU.InterpretOp();
}

unsigned Emulator::Init(const std::string & in_ROMName)
{
	// FIXME : Ain't that ugly
	if(LoadROM(in_ROMName))
		if(m_CPU.Init(m_ROMData[4]))
			if(m_GPU.Init())
				if(m_SPU.Init())
					return NoError;
				else return SPUError;
			else return GPUError;
		else return CPUError;
	else
		return FileError;
}

bool Emulator::LoadROM(const std::string & in_ROMName)
{
	std::ifstream l_FileStream(in_ROMName, std::ios::in | std::ios::binary);
	bool l_Success = false;

	if(l_FileStream.is_open())
	{
		m_ROMData.insert(m_ROMData.begin(), 
			std::istream_iterator<unsigned char>(l_FileStream), 
			std::istream_iterator<unsigned char>());

		l_FileStream.close();
		l_Success = true;
	}

	return l_Success;
}
