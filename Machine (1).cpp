//*******************************
//
// Kush Gandhi and Oscar Vasquez
// CSCI 480-PE1 MidOS Project
//
//*******************************

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <mutex>
#include <stack>
#include <condition_variable>
#include <thread>
#include <chrono>
#include "CPU.h"
#include "Globals.h"


using namespace std;

std::stack<int> systemStack;

//constructor
CPU::CPU()
{
	registers.resize(15);
	current_process_Id = 0;
	pointer = 0;
	stack_pointer = 0;
	clock_tick = 0;
	instruction_pointer = 0;
	global_memory_start = 0;
	sign_flag = false;
	zero_flag = false;
	registerd[16];
	//sets all registers to null
	for (int i = 0; i < 16; i++) {
		registerd[i] = nullptr;
	}
}

CPU::~CPU() {}

// Execute instructions based on the opcode and opreand 
void CPU::perform_instructions(string& opcodestr, int operand1, int operand2)
{
	//enum object
	Opcode op;

	//takes the string opcode read from file and matches it with assembly instructions in the enum
	if (opcodestr == "incr") { op = Opcode::INCR; }
	else if (opcodestr == "addi") { op = Opcode::ADDI; }
	else if (opcodestr == "addr") { op = Opcode::ADDR; }
	else if (opcodestr == "pushr") { op = Opcode::PUSHR; }
	else if (opcodestr == "pushi") { op = Opcode::PUSHI; }
	else if (opcodestr == "movi") { op = Opcode::MOVI; }
	else if (opcodestr == "movr") { op = Opcode::MOVR; }
	else if (opcodestr == "movmr") { op = Opcode::MOVMR; }
	else if (opcodestr == "movrm") { op = Opcode::MOVRM; }
	else if (opcodestr == "movmm") { op = Opcode::MOVMM; }
	else if (opcodestr == "printr") { op = Opcode::PRINTR; }
	else if (opcodestr == "printm") { op = Opcode::PRINTM; }
	else if (opcodestr == "printcr") { op = Opcode::PRINTCR; }
	else if (opcodestr == "printm") { op = Opcode::PRINTCM; }
	else if (opcodestr == "jmp") { op = Opcode::JMP; }
	else if (opcodestr == "jmpi") { op = Opcode::JMPI; }
	else if (opcodestr == "jmpa") { op = Opcode::JMPA; }
	else if (opcodestr == "cmpi") { op = Opcode::CMPI; }
	else if (opcodestr == "cmpr") { op = Opcode::CMPR; }
	else if (opcodestr == "jlt") { op = Opcode::JLT; }
	else if (opcodestr == "jlti") { op = Opcode::JLTI; }
	else if (opcodestr == "jgt") { op = Opcode::JGT; }
	else if (opcodestr == "jgti") { op = Opcode::JGTI; }
	else if (opcodestr == "jgta") { op = Opcode::JGTA; }
	else if (opcodestr == "je") { op = Opcode::JE; }
	else if (opcodestr == "jei") { op = Opcode::JEI; }
	else if (opcodestr == "jea") { op = Opcode::JEA; }
	else if (opcodestr == "call") { op = Opcode::CALL; }
	else if (opcodestr == "callm") { op = Opcode::CALLM; }
	else if (opcodestr == "ret") { op = Opcode::RET; }
	else if (opcodestr == "exit") { op = Opcode::EXIT; }
	else if (opcodestr == "jlt") { op = Opcode::JLT; }
	else if (opcodestr == "popr") { op = Opcode::POPR; }
	else if (opcodestr == "popm") { op = Opcode::POPM; }
	else if (opcodestr == "sleep") { op = Opcode::SLEEP; }
	else if (opcodestr == "input") { op = Opcode::INPUT; }
	else if (opcodestr == "inputc") { op = Opcode::INPUTC; }
	else if (opcodestr == "setpriority") { op = Opcode::SETPRIORITY; }
	else if (opcodestr == "setpriorityl") { op = Opcode::SETPRIORITYI; }
	//should not reach this code 
	else { cout << "error, no opcode detected" << endl; }

	//take the opcode and execute each instruction
	switch (op)
	{
	case INCR:
		executeINCR(operand1);
		break;

	case ADDI:
		executeADDI(operand1, operand2);
		break;

	case ADDR:
		executeADDR(operand1, operand2);
		break;

	case PUSHR:
		pushRegister(operand1);
		break;

	case PUSHI:
		pushConstant(operand1);
		break;

	case MOVI:
		executeMOVI(operand1, operand2);
		break;

	case MOVR:
		executeMOVR(operand1, operand2);
		break;

	case MOVMR:
		executeMOVMR(operand1, operand2);
		break;

	case MOVRM:
		executeMOVRM(operand1, operand2);
		break;

	case MOVMM:
		executeMOVMM(operand1, operand2);
		break;

	case PRINTR:
		executePRINTR(operand1);
		break;

	case PRINTM:
		executePRINTM(operand1);
		break;

	case PRINTCR:
		executePRINTCR(operand1);
		break;

	case JMP:
		executeJMP(operand1);
		break;

	case JMPI:
		executeJMPI(operand1);
		break;

	case JMPA:
		executeJMPA(operand1);
		break;

	case CMPI:
		executeCMPI(operand1, operand2);
		break;

	case CMPR:
		executeCMPR(operand1, operand2);
		break;

	case JLT:
		executeJLT(operand1);
		break;

	case JLTI:
		executeJLTI(operand1);
		break;

	case JLTA:
		executeJLTA(operand1);
		break;

	case JGT:
		executeJGT(operand1);
		break;

	case JGTI:
		executeJGTI(operand1);
		break;

	case JGTA:
		executeJGTA(operand1);
		break;

	case JE:
		executeJE(operand1);
		break;

	case JEI:
		executeJEI(operand1);
		break;

	case JEA:
		executeJEA(operand1);
		break;

	case CALL:
		executeCALL(operand1);
		break;

	case CALLM:
		executeCALLM(operand1);
		break;

	case RET:
		executeRET();
		break;

	case EXIT:
		executeEXIT();
		break;

	case POPR:
		executePOPR(operand1);
		break;

	case POPM:
		executePOPM(operand1);
		break;

	case SLEEP:
		executeSLEEP(operand1);
		break;

	case INPUT:
		executeINPUT(operand1);
		break;

	case INPUTC:
		executeINPUTC(operand1);
		break;

	case SETPRIORITY:
		executeSETPRIORITY(operand1);
		break;

	case SETPRIORITYI:
		executeSETPRIORITYI(operand1);
		break;

	default:
		cout << "Unknown opcode" << endl;
	}
	clock_tick++; // Increments system clock after each instructions
}

//implements all methods of the opcodes here.
void CPU::executeINCR(int reg_num)
{
	registers[reg_num]++;
	cout << "Incr register called" << endl;
}

void CPU::executeADDI(int reg_num, int constant)
{
	registers[reg_num] += constant;
}

void CPU::executeADDR(int reg_num, int reg_num1)
{
	registers[reg_num] += registers[reg_num1];
}

//Push rx onto the stack. Decrement SP by 4.
void CPU::pushRegister(int reg_num)
{
	//pushing register onto stack
	if (reg_num >= 1 && reg_num <= 14)
	{
		stack_pointer -= 4;
	}
}

//Pushes the constant x onto the stack. Decrement SP by 4
void CPU::pushConstant(int constant)
{
	stack_pointer -= 4;
}

//moves constant value to reg number
void CPU::executeMOVI(int reg_num, int constant)
{
	registers[reg_num] = constant;
}

//moves reg num2 to reg num1
void CPU::executeMOVR(int reg_num1, int reg_num2)
{
	registers[reg_num1] = registers[reg_num2];
}

//indicates contents stored at the address in ry
void CPU::executeMOVMR(int reg_num1, int reg_num2)
{
	registers[reg_num1] = global_memory_start + registers[reg_num2];
}

//moves reg2 to reg 1
void CPU::executeMOVRM(int reg_num1, int reg_num2)
{
	registers[reg_num1] = registers[reg_num2];
}

//memory-to-memory move
void CPU::executeMOVMM(int reg_num1, int reg_num2)
{
	registers[reg_num1] = global_memory_start + registers[reg_num2];
}

//Display contents of register 
void CPU::executePRINTR(int reg_num)
{
	cout << registers[reg_num] << endl;
}

//Display memory contents at the address in register x.
void CPU::executePRINTM(int address)
{
	cout << "Memory Contant as the address is " << address << endl;
}

//Displays contents of register 1 as a character
void CPU::executePRINTCR(int reg_num)
{
	cout << reg_num << static_cast<char>(registers[reg_num]) << endl;
}

//Display memory contents at the address in register x as a character
void CPU::executePRINTCM(int address)
{
	cout << "Memory Contant as the address is " << address << endl;
}

//Transfer control to the instruction whose address is rx bytes relative to the current instruction. 
// Rx may be negative.For example: If r1 = 10, jmp r1 will add 10 bytes to the instruction pointer.
void CPU::executeJMP(int address)
{
	instruction_pointer += address;
}

//Transfer control to the instruction with offset x from the current IP
void CPU::executeJMPI(int offset)
{
	instruction_pointer += offset;
}

//Transfer control to the absolute address x.
void CPU::executeJMPA(int address)
{
	instruction_pointer = address;
}

//Subtract y from register rx. 
// If rx < y, set the sign flag. If rx > y, clear the sign flag. If rx == y, set zero flag.
void CPU::executeCMPI(int reg_num, int constant)
{
	if (registers[reg_num] < constant)
	{
		sign_flag = true;
		zero_flag = false;
	}
	else if (registers[reg_num] > constant)
	{
		sign_flag = false;
		zero_flag = true;
	}
	else {
		sign_flag = false;
		zero_flag = true;
	}
}

//Like cmpi, except now both operands are registers.
void CPU::executeCMPR(int reg_num1, int reg_num2)
{
	if (registers[reg_num1] < registers[reg_num2])
	{
		sign_flag = true;
		zero_flag = false;
	}
	else if (registers[reg_num1] > registers[reg_num2])
	{
		sign_flag = false;
		zero_flag = true;
	}
	else {
		sign_flag = false;
		zero_flag = true;
	}
}

//If the sign flag is set, jump to the instruction whose offset is rx bytes from the current instruction.
void CPU::executeJLT(int reg_num)
{
	// Check if the zero flag is clear.
	if (sign_flag)
	{
		instruction_pointer += registers[reg_num];
	}
}

//If the sign flag is set, jump to the instruction whose offset is x bytes from the current instruction.
void CPU::executeJLTI(int reg_num)
{
	// Check if the zero flag is clear.
	if (sign_flag)
	{
		instruction_pointer += reg_num;
	}
}

//If the sign flag is set, jump to address x.
void CPU::executeJLTA(int address)
{
	// Check if the zero flag is clear.
	if (sign_flag)
	{
		// If the zero flag is clear, jump to the specified address
		instruction_pointer = address;
	}
}

//if the sign flag is clear, jump to the instruction whose offset is rx bytes from the current instruction
void CPU::executeJGT(int offset)
{
	// Check if the zero flag is clear.
	if (!sign_flag)
	{
		// Increment the instruction pointer by the specified offset to move to the next instruction.
		instruction_pointer += offset;
	}
}

//if the sign flag is clear, jump to the instruction whose offset is x bytes from the current instruction
void CPU::executeJGTI(int offset)
{
	// Check if the zero flag is clear.
	if (!sign_flag)
	{
		// Increment the instruction pointer by the specified offset to move to the next instruction.
		instruction_pointer = offset;
	}
}

//If the sign flag is clear, jump to address x.
void CPU::executeJGTA(int address)
{
	// Check if the zero flag is clear.
	if (!sign_flag)
	{
		// If the zero flag is clear, jump to the specified address
		instruction_pointer = address;
	}
}

//if the zero flag is clear, jump to the instruction whose offset is rx bytes from the current instruction.
void CPU::executeJE(int offset)
{
	// Check if the zero flag is clear.
	if (!zero_flag)
	{
		// Increment the instruction pointer by the specified offset to move to the next instruction.
		instruction_pointer += offset;
	}
}

//if the zero flag is clear, jump to the instruction whose offset is x bytes from the current instruction.
void CPU::executeJEI(int offset)
{
	// Check if the zero flag is clear.
	if (!zero_flag)
	{
		// Increment the instruction pointer by the specified offset to move to the next instruction.
		instruction_pointer += offset;
	}

}

//if the zero flag is clear, jump to address x
void CPU::executeJEA(int address)
{
	// Check if the zero flag is clear.
	if (!zero_flag)
	{
		// If the zero flag is clear, jump to the specified address
		instruction_pointer = address;
	}
}
void CPU::executeCALL(int offset)
{
	// Push the return address onto the stack
	stack_pointer -= 4;

	// Store the address of the next instruction after the call
	int return_address = instruction_pointer + offset;

	registers[10] = return_address;
}

void CPU::executeCALLM(int reg_num)
{
	// Push the return address onto the stack
	stack_pointer -= 4;

	// Registers are 1-indexed, so accessing register 11 is at index 10
	registers[10] = instruction_pointer;

	// Set the instruction pointer to the address of the procedure
	//instruction_pointer = procedure_address;
}

void CPU::executeRET()
{
	// Initialize return_address variable
	int return_address = 0;

	// Check if the stack pointer is within the bounds of the system stack.
	if (stack_pointer < systemStack.size())
	{
		// Retrieve the return address from the top of the system stack.
		int return_address = systemStack.top();
		// Remove the return address from the system stack.
		systemStack.pop();
		// Set the instruction pointer to the retrieved return address.
		instruction_pointer = return_address;
	}
	else
	{
		// If stack underflow occurs, print an error message.
		cerr << "Stack underflow error" << endl;
	}
}

void CPU::executeEXIT()
{
	cout << "Exit Function called. Process exiting." << endl;

	PCB process;

	//unload the process
	memory->unload_process();

	//schedule the next process
	memory->schedule_process(process);
}

void CPU::executePOPR(int reg_num)
{
	//check within the valid range
	if (stack_pointer < 0 || reg_num < 0) {
		cerr << "Error: out of bounds" << endl;
	}

	//store the index to the top of stack
	registers[reg_num] = systemStack.top();
	systemStack.pop();
	stack_pointer += 4;
}

void CPU::executePOPM(int reg_num)
{
	// Pop the top of the stack into the specified register
	if (stack_pointer >= 0)
	{
		registers[reg_num] = systemStack.top();

		// decrement the stack pointer
		stack_pointer -= 4;
	}
}

void CPU::executeSLEEP(int cycles)
{
	cout << "Sleeping for " << cycles << " seconds" << endl;
	this_thread::sleep_for(chrono::seconds(cycles));
}

void CPU::executeINPUT(int reg_num)
{
	//Reading a single input from the keyboard
	int input_value;
	std::cout << "Enter Input: ";
	std::cin >> input_value;

	// Store the input value in the specified register
	registers[reg_num] = input_value;
}

void CPU::executeINPUTC(int reg_num)
{
	//Reading a single character input from the keyboard
	char input_char;
	cout << "Enter a character: ";
	cin >> input_char;

	// Convert the character to its ASCII value and store it in the specified register
	registers[reg_num] = static_cast<int>(input_char);
}

void CPU::executeSETPRIORITY(int reg_num)
{
	//get the priority value from the registers
	int priority = registers[reg_num];
}

void CPU::executeSETPRIORITYI(int reg_num)
{
	//get the priority value from the registers
	int priority = registers[reg_num];
}

// Map the value from register rx to register ry and shared memory.
void CPU::MapSharedMem(int rx, int ry)
{
	// Copy the value from register rx to register ry.
	// Store the value of register rx into shared memory.
	// Store the starting address of shared memory into register ry.
	registers[ry] = registers[rx];
	int sharedMemory = registers[rx];
	int startAddress = registers[ry];
}

// Acquire a lock identified by lockNumber.
void CPU::AcquireLock(int lockNumber)
{
	// Static vector to hold mutex locks.
	static vector<mutex> locks;
	// Check if the lockNumber is within a valid range.
	if (locks.size() < 0 || locks.size() > 10)
	{
		cerr << "error: no lock detected" << endl;
	}
	// Lock the specified lockNumber.
	if (lockNumber > 0)
	{
		locks[lockNumber].lock();
	}
}

// Release the lock identified by lockNumber.
void CPU::ReleaseLock(int lockNumber)
{
	// Static vector to hold mutex locks
	// Unlock the specified lockNumber.
	static vector<mutex> locks;
	if (lockNumber > 0) {
		locks[lockNumber].unlock();
	}
}

// Wait for an event identified by param.
void CPU::WaitEvent(int param)
{
	// Static condition variable and mutex to synchronize events.
	static condition_variable c;
	static mutex mtx;
	unique_lock<mutex> lck(mtx);
	//Check if param is within a valid range.
	if (param > 0 && param < 10)
	{
		// Wait until the eventState[param] is set to 1.
		while (eventState[param] == 0)
		{
			c.wait(lck);
		}
		// Once the eventState[param] is set to 1, update it to indicate event completion.
		eventState[param] = 1;
	}
}

// Signal an event identified by param.
void CPU::SignalEvent(int param)
{
	// Static condition variable and mutex to synchronize events.
	static condition_variable c;
	static mutex mtx;
	static unique_lock<mutex> lck(mtx);
	// Check if param is within a valid range.
	if (param > 0 && param < 10)
	{
		// Set the eventState[param] to 1 to signal completion of the event.
		eventState[param] = 1;
		// Notify all waiting threads that the event has occurred.
		c.notify_all();
	}
}

// Allocate memory and store the address in a register.
void CPU::Alloc(int rx, int ry)
{
	// allocate memory based on size specified in register rx.
	int* allocateMemory = new int[rx];

	// Check if memory allocation failed.
	if (allocateMemory == nullptr)
	{
		// Clear the register if memory allocation fails.
		registerd[ry] = nullptr; //clear the register
		cout << "Memory allocation has failed." << endl;
	}
	else
	{
		// Store the allocated memory address in the specified register.
		registerd[ry] = allocateMemory; //store the address
		memory->allocateHeapMemory(rx, ry);
		cout << "Memory has been allocated." << endl;
	}
}

// Free memory allocated at a specific register.
void CPU::FreeMemory(int ry)
{
	// Get the address of the memory to be freed.
	int* mem = &ry;
	// Check if there is memory allocated in the specified register.
	if (registerd != nullptr)
	{
		// Free the memory and clear the register.
		delete mem; // Free Memory
		delete[] registerd[ry]; // Delete allocated memory
		registerd[ry] = nullptr; // Clear the register
		memory->freeHeapMemory(ry); //Track deallocation in memory manager
		cout << "The Memory is free from the system." << endl;
	}
	else
	{
		// Print a message if no memory is found in the specified register.
		cout << "No memory in register " << ry << endl;
	}

}

//connect to the memory class
void CPU::connectMemory(Memory& mem) { memory = &mem; }


void CPU::flags(int result)
{
	// Set sign and zero flags based on result
	sign_flag = result < 0;
	zero_flag = result == 0;
}

void CPU::read_file()
{
	ifstream data_file;
	string fileName;
	string opcode;
	string param1;
	string param2;
	string data_line;

	cout << "Enter Data File: ";
	getline(cin, fileName);
	data_file.open(fileName);

	//checks for invalid file
	while (!data_file)
	{
		data_file.close();
		data_file.clear();
		cout << "Unable to open file! '" << fileName << "'. Please enter another name: ";
		getline(cin, fileName);
		data_file.open(fileName);
	}

	//ensures that file is open
	cout << "File '" << fileName << "' opened successfully!" << endl;

	//loop till end of file
	while (!data_file.eof())
	{
		//read and store data from file
		getline(data_file, data_line);
		stringstream ss(data_line);

		//read in the opcode and the parameters
		ss >> opcode;
		ss >> param1;
		ss >> param2;

		//removes and erases , and ; from the parameters
		param1.erase(remove(param1.begin(), param1.end(), ','), param1.end());
		param1.erase(remove(param1.begin(), param1.end(), ';'), param1.end());
		param2.erase(remove(param2.begin(), param2.end(), ','), param2.end());
		param2.erase(remove(param2.begin(), param2.end(), ';'), param2.end());

		//if the string contains $ or r then we only to read the value by substring
		if (param1[0] == '$' || param1[0] == 'r' || param1[0] == '#')
		{
			param1 = param1.substr(1);
		}
		if (param2[0] == '$' || param2[0] == 'r' || param2[0] == '#')
		{
			param2 = param2.substr(1);
		}

		//convert the string to int for register notation
		int p1 = 0;
		int p2 = 0;

		if (!param1.empty())
		{
			p1 = stoi(param1);
		}
		if (!param2.empty())
		{
			p2 = stoi(param2);
		}

		//converts the opcode in lowercase format
		transform(opcode.begin(), opcode.end(), opcode.begin(), [](unsigned char c) { return tolower(c); });

		//call to perform instructions
		perform_instructions(opcode, p1, p2);
		//testing files with example assembly instructions
		//cout << "Opcode: " << opcode << " Param1: " << param1 << " int p1: " << p1 << " Param2: " << param2 << " int p2: " << p2 << endl;
	}
	data_file.close();
}
