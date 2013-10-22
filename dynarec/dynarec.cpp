#include <iostream>
#include <map>
#include <memory>
#include <vector>

class Dynarec
{
private:
    struct CPUState
    {
        unsigned char PC;
        unsigned short Reg1;
        unsigned short Reg2;
        
        CPUState() : PC(0), Reg1(0), Reg2(0) { }
        ~CPUState() { }
    };

private:
    typedef void (Dynarec::*CompileFunc)();
        
private:
    static const CompileFunc m_OpcodeTable[1] = { &Dynarec:: };
    std::map<unsigned char, std::unique_ptr<unsigned char>> m_TranslationCache;
    CPUState m_CPU;
    
private:
    // Compile a basic block starting from the PC 
    void Compile()
    {
         m_TranslationCache[m_CPU.PC] = (this->mOpcodeTable[m_CPU.PC])();
    }
    
    void Execute(const std::unique_ptr<unsigned char> & in_Ins)
    {
        // Cast to function pointer and execute
        ((void(*)())in_Ins)();
    }
    
private:
    void 
        
public:
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
            Execute(m_TranslationCache[m_CPU.PC]);
        }
    }
};

int main()
{
    std::cout << "Hello world" << std::endl;
    // vector of instructions
}