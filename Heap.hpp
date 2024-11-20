#ifndef HEAP_HPP
#define HEAP_HPP

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "MemoryBlock.hpp"

struct Heap {
    CRITICAL_SECTION critSection;  // Heap-specific critical section
    MemoryBlock* allocations;      // Linked list of allocations
    size_t totalAllocations;       // Number of active allocations

    Heap() : allocations(nullptr), totalAllocations(0) {
        InitializeCriticalSection(&critSection);
    }

    ~Heap() {
        while (allocations) {
            MemoryBlock* next = allocations->next;
            free(allocations);
            allocations = next;
        }
        DeleteCriticalSection(&critSection);
    }
};

#endif // !HEAP_HPP