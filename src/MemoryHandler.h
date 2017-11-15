#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>

class MemoryHandler
{
public:

    MemoryHandler();
    ~MemoryHandler();

    void Initialize();
    std::string AllocateMemory();

    unsigned int system_memory;
    unsigned int block_size;
    unsigned int last_block;
};

#endif
