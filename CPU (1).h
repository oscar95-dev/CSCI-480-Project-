#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include "Memory.h"

//array of events set to 1 initially
static int eventState[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };

enum Opcode {
	INCR,
	ADDI,
	ADDR,
	PUSHR,
	PUSHI,
	MOVI,
	MOVR,
	MOVMR,
	MOVRM,
	MOVMM,
	PRINTR,
	PRINTM,
	PRINTCR,
	PRINTCM,
	JMP,
	JMPI,
	JMPA,
	CMPI,
	CMPR,
	JLT,
	JLTI,
	JLTA,
	JGT,
	JGTI,
	JGTA,
	JE,
	JEI,
	JEA,
	CALL,
	CALLM,
	RET,
	EXIT,
	POPR,
	POPM,
	SLEEP,
	INPUT,
	INPUTC,
	SETPRIORITY,
	SETPRIORITYI
};

class CPU
{
private:
	std::vector<int> registers;
	int* registerd[16];
	int pointer;
	int current_process_Id;
	int stack_pointer;
	int clock_tick;
	int instruction_pointer;
	int global_memory_start;
	bool sign_flag;
	bool zero_flag;
	Memory* memory;

public:

	CPU();
	~CPU();
	void perform_instructions(std::string& opcodestr, int operand1, int operand2);
	void executeINCR(int reg_num);
	void executeADDI(int reg_num, int constant);
	void executeADDR(int reg_num, int reg_num1);
	void pushRegister(int reg_num);
	void pushConstant(int constant);
	void executeMOVI(int reg_num, int constant);
	void executeMOVR(int reg_num1, int reg_num2);
	void executeMOVMR(int reg_num1, int reg_num2);
	void executeMOVRM(int reg_num1, int reg_num2);
	void executeMOVMM(int reg_num1, int reg_num2);
	void executePRINTR(int reg_num);
	void executePRINTM(int reg_num);
	void executePRINTCR(int reg_num);
	void executePRINTCM(int address);
	void executeJMP(int address);
	void executeJMPI(int offset);
	void executeJMPA(int address);
	void executeCMPI(int reg_num, int constant);
	void executeCMPR(int reg_num1, int reg_num2);
	void executeJLT(int reg_num);
	void executeJLTI(int reg_num);
	void executeJLTA(int reg_num);
	void executeJGT(int offset);
	void executeJGTI(int offset);
	void executeJGTA(int address);
	void executeJE(int offset);
	void executeJEI(int offset);
	void executeJEA(int address);
	void executeCALL(int offset);
	void executeCALLM(int offset);
	void executeRET();
	void executeEXIT();
	void executePOPR(int reg_num);
	void executePOPM(int reg_num);
	void executeSLEEP(int cycles);
	void executeINPUT(int reg_num);
	void executeINPUTC(int reg_num);
	void executeSETPRIORITY(int reg_num);
	void executeSETPRIORITYI(int reg_num);
	void MapSharedMem(int rx, int ry);
	void AcquireLock(int lockNumber);
	void ReleaseLock(int lockNumber);
	void WaitEvent(int param);
	void SignalEvent(int param);
	void flags(int result);
	void read_file();
	void Alloc(int rx, int ry);
	void FreeMemory(int rx);
	void connectMemory(Memory& mem);
};
#endif // CPU_H_INCLUDED
