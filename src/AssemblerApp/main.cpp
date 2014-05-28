#include <iostream>

#include "assembler.h"

int main(int argc, char** argv)
{
    if(argc > 1)
    {
        Assembler l_Asm;
        l_Asm.Assemble(argv[1]);
    }
    else
    {
        std::cout << "ERROR: No assembly file given" << std::endl;
    }

    return 0;
}

