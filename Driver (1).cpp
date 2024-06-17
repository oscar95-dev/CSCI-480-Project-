#include <iostream>
#include "CPU.h"
#include "Memory.h"

using namespace std;

int main()
{
	Memory memory;
	CPU cpu;
	PCB p;
	cpu.connectMemory(memory);

	cout << "Welcome to our Mid Operating System!!\n\n" << endl;

	cpu.read_file();
	memory.print_stats(p);
	return 0;
}
