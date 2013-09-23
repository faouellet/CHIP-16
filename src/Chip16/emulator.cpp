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
	auto l_ROMData = ReadROM(in_ROMName);
	if(!l_ROMData.empty())
		if(m_CPU.Init(std::move(l_ROMData)))
			if(m_GPU.Init())
				if(m_SPU.Init())
					return NoError;
				else return SPUError;
			else return GPUError;
		else return CPUError;
	else
		return FileError;
}

std::vector<const UInt8> Emulator::ReadROM(const std::string & in_ROMName)
{
	std::ifstream l_FileStream(in_ROMName, std::ios::in | std::ios::binary);
	std::vector<const UInt8> l_ROMData;

	if(l_FileStream.is_open())
	{
		l_ROMData.insert(l_ROMData.begin(), 
			std::istream_iterator<unsigned char>(l_FileStream), 
			std::istream_iterator<unsigned char>());
			
		l_FileStream.close();
	}

	return l_ROMData;
}
