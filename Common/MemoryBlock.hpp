#ifndef MEMORY_BLOCK_HPP
#define MEMORY_BLOCK_HPP

struct MemoryBlock {
    void* ptr;             // Pointer to allocated memory
    size_t size;           // Size of allocation
    size_t heapIndex;      // Which heap owns this block
    MemoryBlock* next;     // Next block in list

    MemoryBlock(void* p, size_t s, size_t hi)
        : ptr(p), size(s), heapIndex(hi), next(nullptr) {
    }
};

#endif // !MEMORY_BLOCK_HPP