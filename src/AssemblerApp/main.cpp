#include <iostream>

#include "assembler.h"

int main(int argc, char** argv)
{
    if(argc > 1)
    {
        Assembler l_Asm;
		if (!l_Asm.Assemble(argv[1]))
			std::cout << "ERROR: Could not assemble the given file" << std::endl;
    }
    else
    {
        std::cout << "ERROR: No assembly file given" << std::endl;
    }

    return 0;
}

