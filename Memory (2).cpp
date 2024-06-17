#include <iostream>
#include <algorithm>
#include <vector>

#include "CPU.h"
#include "Memory.h"

using namespace std;

Memory::Memory()
{
    address = 0;
    size = 0;

    // Initialize virtual page table with empty pages
    virtual_page_table.resize(virtual_page_count);
    for (int i = 0; i < virtual_page_count; ++i) {
        virtual_page_table[i].resize(1); // Each virtual page can hold one process
        virtual_page_table[i][0].IsValid = false; // Mark all pages as invalid initially
    }
}

// Destructor for the Memory class.
Memory::~Memory()
{
    // Calculate the number of physical pages needed.
    size_t physicalPagesNeeded = physical_memory.size() / page_size;
    // Resize the virtual page table to accommodate the calculated physical pages.
    virtual_page_table.resize(physicalPagesNeeded);

    // Loop through the virtual page table.
    for (int i = 0; i < virtual_page_table.size(); i++)
    {
        // Loop through each entry in the virtual page table.
        for (int j = 0; j < virtual_page_table[i].size(); j++)
        {
            // Create a new Page object
            Page page;
            // Set the flags of the page.
            virtual_page_table[i][j].IsDirty = false;
            virtual_page_table[i][j].IsValid = true;
            // Set the process_id of the page.
            page.process_id = 0;
            // Add the page to the virtual page table
            virtual_page_table[i].push_back(page);
        }
    }
}

void Memory::schedule_process(const PCB& process)
{
    //push the process into the queue
    readyQueue.push(process);

    //move the process to a temp vector to sort through
    vector<PCB> temp;
    while (!readyQueue.empty()) {
        temp.push_back(readyQueue.front());
        readyQueue.pop();
    }

    //sort the queue based on priority
    sort(temp.begin(), temp.end(), [](const PCB& a, const PCB& b)
        {
            return a.Priority > b.Priority;
        });

    //transfer the elements back to readyQueue
    for (auto& PCB : temp) {
        readyQueue.push(PCB);
    }
}

void Memory::writeMemory(int address, int value)
{
    if (address < 0 || address >= physical_memory.size())
    {
        std::cerr << "error" << std::endl;
    }

    int page = (address >> 8) & 0xFFFFFF; // Extract the 24-bit page
    int offset = address & 0xFF; // Extract the 8-bit offset

    //look up the specific memory address
    int physical_address = page * page_size + offset;
    //set the address to the value
    physical_memory[physical_address] = value;
}

int Memory::readMemory(int address) const
{
    //check if addres is wihtin in memory
    if (address < 0 || address >= physical_memory.size())
    {
        std::cerr << "error" << std::endl;
        return -1; //returns the error state
    }
    int page = (address >> 8) & 0xFFFFFF; // Extract the 24-bit page

    int offset = address & 0xFF; // Extract the 8-bit offset

    //look up the specific memory address
    int physical_address = page * page_size + offset;

    return physical_memory[physical_address];
}


void Memory::print_stats(const PCB& process) const
{
    cout << "Process ID: " << process.ProcessID << endl;
    cout << "Page number: " << page_num << endl;
    cout << "Number of context switches: " << context_switch << endl;
}

// Function to load program into memory for a given process
void Memory::load_program(const PCB& process, const std::vector<int>& program)
{
    size_t newIndexPage = 0;

    // Calculate the number of pages required for the program
    size_t num_pages_needed = (program.size() + page_size - 1) / page_size;

}

void Memory::execute_processes()
{
    while (!readyQueue.empty())
    {
        PCB currentProcess = readyQueue.front();
        readyQueue.pop();

        //check if time has expired
        if (currentProcess.QuantumTime > 0)
        {
            currentProcess.QuantumTime--;
            if (currentProcess.QuantumTime == 0)
            {
                contextSwitch();
                continue; //skip to the next process
            }
        }
        if (currentProcess.SleepCounter == WaitingSleep)
        {
            currentProcess.SleepCounter--;
            if (currentProcess.SleepCounter == 0)
            {
                //if sleep counter reaches 0, then ready the process
                currentProcess.state = Ready;
                schedule_process(currentProcess);
                continue;
            }
        }
        //check if the process has terminated
        if (currentProcess.state == Terminated) {
            continue; //skip to next process
        }
    }
}

void Memory::contextSwitch()
{
    //save the current process
    PCB currentProcess = readyQueue.front();
    readyQueue.pop();

    //load the next process from the PCB
    PCB nextProcess = readyQueue.front();

    //perfrom context switches by swapping the states
    swap(currentProcess.state, nextProcess.state);
    swap(currentProcess.QuantumTime, nextProcess.QuantumTime);
    swap(currentProcess.SleepCounter, nextProcess.SleepCounter);

    // Push the current process back into the ready queue
    readyQueue.push(currentProcess);
}

void Memory::unload_process()
{
    //checks if the queue is empty. if not then remove the process
    while (!readyQueue.empty())
    {
        PCB processID = readyQueue.front();
        if (readyQueue.front().ProcessID)
        {
            readyQueue.pop();
        }
    }
}

void Memory::allocateHeapMemory(int rx, int ry)
{
    size_t size = static_cast<int>(rx);
    Page page;

    HeapSegment allocateMemory = HeapPages[address / page_size];
    allocateMemory.allocated = true; //set the allocation to true
    allocateMemory.size = size; //update the size

    if (allocateMemory.allocated)
    {
        //Page is in physical memory
        page.IsDirty = true; //Mark page as dirty
        cout << "Successfully allocated memory" << size << " bytes at " << address << endl;
    }
    else
    {
        //mark the page not dirty when allocation fails
        cerr << "Error: allocation failed" << endl;
        page.IsDirty = false;
    }

}

void Memory::freeHeapMemory(size_t ry)
{
    HeapSegment deallocateMemory = HeapPages[address / page_size];
    deallocateMemory.allocated = false;
    deallocateMemory.size = 0; //update the size to 0

    cout << "Memory has been freed from the heap" << address << endl;

}

// Accesses memory for a given process with read or write operation.
void Memory::accessMemory(int processID, int address, bool isWriteOperation)
{
    // Calculate the page number and offset within the page.
    int pageNum = address / page_size;
    int offset = address % page_size;

    // Reference to the page in the virtual page table.
    Page& page = virtual_page_table[processID][pageNum];

    // Check if the page is valid.
    if (page.IsValid)
    {
        // If it's a write operation, mark the page as dirty.
        if (isWriteOperation)
        {
            page.IsDirty = true;
        }
        // If it's a read operation, invalidate the page.
        else
        {
            page.IsValid = false;
        }
    }
    // If the page is invalid.
    else
    {
        // Set the page as valid and not dirty.
        virtual_page_table[processID][pageNum].IsValid = true;
        virtual_page_table[processID][pageNum].IsDirty = false;

    }
}
