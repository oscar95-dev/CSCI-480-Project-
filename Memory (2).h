#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <iostream>
#include <vector>
#include <queue>

//Define the ProcessState enumeration
typedef enum ProcessState
{
    New,
    Ready,
    Running,
    WaitingSleep,
    WaitingLock,
    WaitingEvent,
    Terminated
};

typedef struct Page
{
    int process_id; // Process ID that own this page
    bool IsValid; // Flag to indicate if the page is in physical memory
    bool IsDirty; // Flag to indicate if the page has been modified
};

typedef struct HeapSegment
{
    size_t start_address;
    size_t size;
    bool allocated;
};

// Definition of PCB struct
typedef struct PCB
{
    int ProcessID;
    int stack_size;
    int data_size;
    int start_heap;
    int end_heap;
    ProcessState state;
    int sign_flag;
    int zero_flag;
    int Priority;
    int QuantumTime;
    int ClockCycles;
    int SleepCounter;
    int numContextSwitches;
    std::vector<int> WorkingSetPages; // Page descriptors associated with the process
};

class Memory
{
private:

    std::vector<std::vector<HeapSegment>> heap_pages; // Table of heap pages
    std::vector<int> physical_memory;
    std::queue<PCB> readyQueue; // Declare readyQueue as a private member variable
    const int page_size = 256; // Page size
    const int MAX_PHYSICAL_PAGES = 100;
    const int SHARED_MEMORY_SIZE = 2 * page_size; // 2 pages of shared memory
    int page_num = 0;
    int context_switch = 0;
    std::vector<HeapSegment> HeapPages; //table of heap pages
    size_t size;
    size_t address;
    const int virtual_page_count = 100; // Number of virtual pages
    std::vector<std::vector<Page>> virtual_page_table; // Virtual page table
    std::vector<std::vector<Page>> process_page_table;   // Process page table

public:

    Memory();
    ~Memory();
    void schedule_process(const PCB& process);
    void writeMemory(int address, int value);
    int readMemory(int address) const;
    void print_stats(const PCB& process) const;
    void load_program(const PCB& process, const std::vector<int>& program);
    void execute_processes();
    void contextSwitch();
    void unload_process();
    void allocateHeapMemory(int rx, int ry);
    void freeHeapMemory(size_t rx);
    void accessMemory(int size, int address, bool isWriteOperation);
};

#endif // MEMORY_H_INCLUDED
