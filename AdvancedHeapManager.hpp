#ifndef ADVANCED_HEAP_MANAGER_HPP
#define ADVANCED_HEAP_MANAGER_HPP

#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <iomanip> 
#include "Heap.hpp"
#include "MemoryBlock.hpp"
#include "Array.hpp"

using namespace std;

class AdvancedHeapManager {
private:
    Heap* heaps;                 // Array of heaps
    size_t numHeaps;             // Number of heaps
    volatile LONG currentHeap;    // Current heap index for round-robin
    CRITICAL_SECTION globalLock;  // Global lock for thread safety

    // Memory tracking
    MemoryBlock* allBlocks;      // Global list of all allocations
    CRITICAL_SECTION blocksLock; // Lock for allBlocks access
    Array<void*> mem_ptrs;       // Wayback pointers to heap memory segmens (only if needed)

public:
    explicit AdvancedHeapManager(size_t heapCount = 4) : numHeaps(heapCount), currentHeap(0), allBlocks(nullptr) {
        InitializeCriticalSection(&globalLock);
        InitializeCriticalSection(&blocksLock);
        heaps = new Heap[numHeaps];
    }

    ~AdvancedHeapManager() {
        while (allBlocks) {
            MemoryBlock* next = allBlocks->next;
            free(allBlocks->ptr);
            delete allBlocks;
            allBlocks = next;
        }

        delete[] heaps;
        DeleteCriticalSection(&globalLock);
        DeleteCriticalSection(&blocksLock);
    }

    void* Malloc(size_t size) {
        if (size == 0) return nullptr;

        // Get next heap using interlocked increment
        size_t heapIndex = InterlockedIncrement(&currentHeap) % numHeaps;

        // Allocate actual memory
        void* ptr = malloc(size);
        if (!ptr) return nullptr;

        // Create memory block
        MemoryBlock* block = new MemoryBlock(ptr, size, heapIndex);

        // Add to global tracking
        EnterCriticalSection(&blocksLock);
        block->next = allBlocks;
        allBlocks = block;
        LeaveCriticalSection(&blocksLock);

        // Add to heap tracking
        EnterCriticalSection(&heaps[heapIndex].critSection);
        block->next = heaps[heapIndex].allocations;
        heaps[heapIndex].allocations = block;
        heaps[heapIndex].totalAllocations++;
        LeaveCriticalSection(&heaps[heapIndex].critSection);

        return ptr;
    }

    void Free(void* ptr) {
        if (!ptr) return;

        EnterCriticalSection(&blocksLock);

        // Find the block in global list
        MemoryBlock** curr = &allBlocks;
        MemoryBlock* block = nullptr;

        while (*curr) {
            if ((*curr)->ptr == ptr) {
                block = *curr;
                *curr = (*curr)->next;
                break;
            }
            curr = &(*curr)->next;
        }

        if (!block) {
            LeaveCriticalSection(&blocksLock);
            cerr << "Attempting to free invalid pointer\n";
            return;
        }

        size_t heapIndex = block->heapIndex;

        // Remove from heap's allocation list
        EnterCriticalSection(&heaps[heapIndex].critSection);
        curr = &heaps[heapIndex].allocations;
        while (*curr) {
            if ((*curr)->ptr == ptr) {
                *curr = (*curr)->next;
                heaps[heapIndex].totalAllocations--;
                break;
            }
            curr = &(*curr)->next;
        }
        LeaveCriticalSection(&heaps[heapIndex].critSection);

        // Free the actual memory
        free(ptr);
        delete block;

        LeaveCriticalSection(&blocksLock);
    }

    // Show summary
    void PrintStats() {
        cout << "\n------------------------------------\n";
        cout << "|      HEAP\t|     Allocations  |";
        cout << "\n------------------------------------\n";

        for (size_t i = 0; i < numHeaps; i++) {
            EnterCriticalSection(&heaps[i].critSection);
            cout << "|        " << i << "\t|         " << setw(2) << setfill('0') << heaps[i].totalAllocations << "       |\n";
            cout << "------------------------------------\n";
            LeaveCriticalSection(&heaps[i].critSection);
        }
    }
};
#endif // ADVANCED_HEAP_MANAGER_HPP
