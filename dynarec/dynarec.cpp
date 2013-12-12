#include <iostream>
#include <map>
#include <vector>

/**
* \file dynarec.cpp
* \brief This is my attempt at a dynamic recompiler largely inspired by:
*        <a href="http://www.altdevblogaday.com/2011/06/12/jit-cpu-emulation-a-6502-to-x86-dynamic-recompiler-part-1/">h-deb</a>
*/

#if defined(_WIN32)
#include "windows.h"	// For executable memory allocation on Windows
#endif

class Dynarec
{
private:
    struct CPUState
    {
        unsigned char PC;
        unsigned short Reg1;
        unsigned short Reg2;

		unsigned char Memory[64*1024];
        
        CPUState() : PC(0), Reg1(0), Reg2(0) { memset(&Memory, 0, 64*1024); }
        CPUState(const std::vector<unsigned char> & in_ROM) : PC(0), Reg1(0), Reg2(0) 
		{
			memset(&Memory, 0, 64*1024);
			int i = 0;
			for(auto in_Byte : in_ROM)
			{
				Memory[i] = in_Byte;
				++i;
			}
		}
        ~CPUState() { }
    };

	struct X86Code
	{
		unsigned char* Code;
		int Size;

		X86Code(unsigned char* in_Code = nullptr, int in_Size = 0) : Code(in_Code), Size(in_Size) { }
	};

private:
    typedef void (Dynarec::*CompileFunc)();
        
private:
	void* m_CodeBuffer;
	unsigned char* m_CodePtr;
    std::map<unsigned char, CompileFunc> m_CompileTable;
    std::map<unsigned char, X86Code> m_TranslationCache;
    CPUState m_CPU;
    
private:
    // Compile a basic block starting from the PC 
    void Compile()
    {
		// Should make a copy of the PC
		// Should run on a separate thread

		// While in a basic block i.e. no jump
		while(((m_CPU.Memory[m_CPU.PC] & 0xF0 >> 4) != 1) && m_CPU.Memory[m_CPU.PC])
		{
			(this->*m_CompileTable[m_CPU.Memory[m_CPU.PC]])();
			m_CPU.PC += 4;
		}
    }
    
    void Execute(const unsigned char * in_Ins)
    {
		__asm{
			pushad;
			call in_Ins;
			popad;
		}
    }
    
private:
    void addimm()
	{
		unsigned char* l_TranslatedBlock = m_CodePtr;

		*m_CodePtr++ = 0x80;
		*m_CodePtr++ = 0xC0;
		*m_CodePtr++ = m_CPU.Memory[m_CPU.PC+2];
		*m_CodePtr++ = 0xC3;	// ret, should be moved to the end of a block
		m_TranslationCache[m_CPU.PC] = X86Code(l_TranslatedBlock, 4);
	}
	        
public:
	Dynarec() : m_CodeBuffer(nullptr), m_CodePtr(nullptr)
	{
		m_CompileTable[0x40] = &Dynarec::addimm;
	}

	Dynarec(const std::vector<unsigned char> & in_ROM) : m_CPU(in_ROM)
	{
		m_CompileTable[0x40] = &Dynarec::addimm;
	}

	~Dynarec()
	{
		VirtualFree(m_CodeBuffer, 0x400, MEM_DECOMMIT | MEM_RELEASE);
	}

	void InitCodeBuffer()
	{
#if defined(_WIN32)
		m_CodeBuffer = VirtualAlloc(0, 0x400, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
		// TODO : POSIX
#endif
		m_CodePtr = reinterpret_cast<unsigned char*>(m_CodeBuffer);
	}

    void ExecuteOp()
    {
        auto & l_NativeIns = m_TranslationCache[m_CPU.PC];
		if(l_NativeIns.Code)
        {
			Execute(l_NativeIns.Code);
        }
        else
        {
            Compile();
			Execute(m_TranslationCache[m_CPU.Memory[m_CPU.PC]].Code);
        }
    }
};

int main()
{
    std::cout << "Hello world" << std::endl;
	std::vector<unsigned char> l_ROM;
	l_ROM.push_back(0x40);
	l_ROM.push_back(0x00);
	l_ROM.push_back(0xFF);
	l_ROM.push_back(0x00);

	Dynarec l_Dynarec(l_ROM);
	l_Dynarec.InitCodeBuffer();
	l_Dynarec.ExecuteOp();
}
