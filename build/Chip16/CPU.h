#ifndef CPU_H
#define CPU_H

class CPU
{
private:
	// TODO : Registers

public:
	CPU();
	~CPU();

	void Emulate();
	void Reset();
};

#endif // CPU_H
