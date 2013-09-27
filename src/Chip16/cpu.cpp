#include "cpu.h"

#include "spu.h"

CPU::CPU() 
{
	RegisterOpCode<NOP,		>();
	RegisterOpCode<CLS,		>();
	RegisterOpCode<VBLNK,	>();
	RegisterOpCode<BGC,		>();
	RegisterOpCode<SPR,		>();
	RegisterOpCode<DRWXY,	>();
	RegisterOpCode<DRWXY,	>();
	RegisterOpCode<RND,		>();
	RegisterOpCode<FLIP00,	>();
	RegisterOpCode<FLIP01,	>();
	RegisterOpCode<FLIP10,	>();
	RegisterOpCode<FLIP11,	>();
	RegisterOpCode<SND0,	>();
	RegisterOpCode<SND1,	>();
	RegisterOpCode<SND2,	>();
	RegisterOpCode<SND3,	>();
	RegisterOpCode<SNP,		>();
	RegisterOpCode<SNG,		>();
	RegisterOpCode<JMP,		>();
	RegisterOpCode<JX,		>();
	RegisterOpCode<JME,		>();
	RegisterOpCode<JMPX,	>();
	RegisterOpCode<CALL,	>();
	RegisterOpCode<RET,		>();
	RegisterOpCode<CX,		>();
	RegisterOpCode<CALLX,	>();
	RegisterOpCode<LDIX,	>();
	RegisterOpCode<LDISP,	>();
	RegisterOpCode<LDMX,	>();
	RegisterOpCode<LDMXY,	>();
	RegisterOpCode<MOV,		>();
	RegisterOpCode<STMX,	StoreInstruction<>>();
	RegisterOpCode<STMXY,	StoreInstruction<>>();
	RegisterOpCode<ADDI,	ArithmeticInstruction<std::plus, ImmediateOpMode>>();
	RegisterOpCode<ADDXY,	ArithmeticInstruction<std::plus, InplaceOpMode>>();
	RegisterOpCode<ADDXYZ,	ArithmeticInstruction<std::plus, StandardOpMode>>();
	RegisterOpCode<SUBI,	ArithmeticInstruction<std::minus, ImmediateOpMode>>();
	RegisterOpCode<SUBXY,	ArithmeticInstruction<std::minus, InplaceOpMode>>();
	RegisterOpCode<SUBXYZ,	ArithmeticInstruction<std::minus, StandardOpMode>>();
	RegisterOpCode<CMPI,	>();
	RegisterOpCode<CMP,		>();
	RegisterOpCode<ANDI,	ArithmeticInstruction<std::bit_and, ImmediateOpMode>>();
	RegisterOpCode<ANDXY,	ArithmeticInstruction<std::bit_and, InplaceOpMode>>();
	RegisterOpCode<ANDXYZ,	ArithmeticInstruction<std::bit_and, StandardOpMode>>();
	RegisterOpCode<TSTI,	>();
	RegisterOpCode<TST,		>();
	RegisterOpCode<ORI,		ArithmeticInstruction<std::bit_or, ImmediateOpMode>>();
	RegisterOpCode<ORXY,	ArithmeticInstruction<std::bit_or, InplaceOpMode>>();
	RegisterOpCode<ORXYZ,	ArithmeticInstruction<std::bit_or, StandardOpMode>>();
	RegisterOpCode<XORI,	ArithmeticInstruction<std::bit_xor, ImmediateOpMode>>();
	RegisterOpCode<XORXY,	ArithmeticInstruction<std::bit_xor, InplaceOpMode>>();
	RegisterOpCode<XORXYZ,	ArithmeticInstruction<std::bit_xor, StandardOpMode>>();
	RegisterOpCode<MULI,	ArithmeticInstruction<std::multiplies, ImmediateOpMode>>();
	RegisterOpCode<MULXY,	ArithmeticInstruction<std::multiplies, InplaceOpMode>>();
	RegisterOpCode<MULXYZ,	ArithmeticInstruction<std::multiplies, StandardOpMode>>();
	RegisterOpCode<DIVI,	ArithmeticInstruction<std::divides, ImmediateOpMode>>();
	RegisterOpCode<DIVXY,	ArithmeticInstruction<std::divides, InplaceOpMode>>();
	RegisterOpCode<DIVXYZ,	ArithmeticInstruction<std::divides, StandardOpMode>>();
	RegisterOpCode<SHLXN,	ArithmeticInstruction<Utils::LeftShift, >>();
	RegisterOpCode<SHRXN,	>();
	RegisterOpCode<SALXN,	>();
	RegisterOpCode<SARXN,	>();
	RegisterOpCode<SHLXY,	>();
	RegisterOpCode<SHRXY,	>();
	RegisterOpCode<SALXY,	>();
	RegisterOpCode<SARXY,	>();
	RegisterOpCode<PUSH,	>();
	RegisterOpCode<POP,		>();
	RegisterOpCode<PUSHALL,	>();
	RegisterOpCode<POPALL,	>();
	RegisterOpCode<PUSHF,	>();
	RegisterOpCode<POPF,	>();
	RegisterOpCode<PAL,		>();
	RegisterOpCode<PALX,	>();
}

CPU::~CPU() { }

bool CPU::Init(std::vector<UInt8> && in_ROMData) 
{
	for(int i = 0; i < HEADER_SIZE; ++i)
		m_ROMHeader[i] = in_ROMData[i];

	for(unsigned i = HEADER_SIZE ; i < in_ROMData.size(); ++i)
		m_Memory[i-HEADER_SIZE] = in_ROMData[i];

	m_PC = m_ROMHeader[0xA];
	m_SP = 0xFDF0;

	return true;
}

void  CPU::Run() 
{
	m_FctTable[FetchOpCode()](); 
}

UInt32 CPU::FetchOpCode()
{
	return (m_Memory[m_PC] >> 8 | m_Memory[m_PC+=1]);
}

UInt16 CPU::GetInputRegisterValue()
{
	return m_Registers[m_Memory[m_PC++] >> 24];
}

void CPU::SetNegativeZeroFlag(UInt16 in_Result)
{
	// Set the zero flag (Bit[2])
	m_FR = in_Result == 0 ? m_FR | 0x4 : m_FR & ~0x4;
	// Set the negative flag (Bit[7])
	m_FR = in_Result & 0x4000 ? m_FR | 0x1000 : m_FR & ~0x4000;
}
