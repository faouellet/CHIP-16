#include <iostream>
#include <map>
#include <vector>

// IDEA : Use a memory arena dor the translation cache

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

private:
    typedef void (Dynarec::*CompileFunc)();
        
private:
    std::map<unsigned char, CompileFunc> m_CompileTable;
    std::map<unsigned char, unsigned char*> m_TranslationCache;
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
        // Cast to function pointer and execute
		((void(*)())in_Ins)();
    }
    
private:
    void addimm()
	{
		unsigned char * l_TranslatedBlock = new unsigned char;

		*l_TranslatedBlock++ = 0x80;
		*l_TranslatedBlock++ = 0xC0;
		*l_TranslatedBlock++ = m_CPU.Memory[m_CPU.PC+2];
		m_TranslationCache[m_CPU.PC] = l_TranslatedBlock;
	}
        
public:
	Dynarec()
	{
		m_CompileTable[0x40] = &Dynarec::addimm;
	}

	Dynarec(const std::vector<unsigned char> & in_ROM) : m_CPU(in_ROM)
	{
		m_CompileTable[0x40] = &Dynarec::addimm;
	}

	~Dynarec()
	{
		for(auto l_IDCache : m_TranslationCache)
			delete l_IDCache.second;
	}

    void ExecuteOp()
    {
        auto & l_NativeIns = m_TranslationCache[m_CPU.PC];
        if(l_NativeIns)
        {
            Execute(l_NativeIns);
        }
        else
        {
            Compile();
            Execute(m_TranslationCache[m_CPU.Memory[m_CPU.PC]]);
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
	l_Dynarec.ExecuteOp();
}