#ifndef ASM_EMITTER_H
#define ASM_EMITTER_H

#include <string>
#include <vector>

#include "../Utils/utils.h"

using Utils::UInt8;
using Utils::UInt32;

/*
 * \class Emitter
 * \brief Produce a binary file encoded with Chip16 instructions
 */
class Emitter
{
private:
    std::vector<UInt32> m_Buffer;

public:
    /*
     * \fn Emitter
     * \brief Default constructor
     * \param in_BufSize Size of the buffer to generate in bytes
     */
    Emitter(const UInt32 & in_BufSize);

    /*
     * \fn ~Emitter
     * \brief Destructor
     */
    ~Emitter();

    void EmitToFile(const std::string & in_Filename) const;

private:
    void EmitInstruction(const UInt8 in_Opcode, const UInt8 in_Op1, 
            const UInt8 in_Op2, const UInt8 in_Op3);

public:    /////////////// Arithmetric ///////////////
    void EmitAdd(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3);
    void EmitAddImm(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitAddInp(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitAnd(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3);
    void EmitAndImm(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitAndInp(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitARShift(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitARShiftImm(const UInt8 in_Op1, const UInt8 in_Op2);
    
    void EmitCmp(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitCmpImm(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitDiv(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3);
    void EmitDivImm(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitDivInp(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitLLShift(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitLLShiftImm(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitLRShift(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitLRShiftImm(const UInt8 in_Op1, const UInt8 in_Op2);
    
    void EmitMul(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3);
    void EmitMulImm(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitMulInp(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitOr(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3);
    void EmitOrImm(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitOrInp(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitSub(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3);
    void EmitSubImm(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitSubInp(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitTst(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitTstImm(const UInt8 in_Op1, const UInt8 in_Op2);

    void EmitXor(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3);
    void EmitXorImm(const UInt8 in_Op1, const UInt8 in_Op2);
    void EmitXorInp(const UInt8 in_Op1, const UInt8 in_Op2);

public:    /////////////// Stack ///////////////
    void EmitPush(const UInt8 in_RegID);
    void EmitPop(const UInt8 in_RegID);
    void EmitPushAll();
    void EmitPopAll();
    void EmitPushF();
    void EmitPopF();
};

#endif // ASM_EMITTER_H

