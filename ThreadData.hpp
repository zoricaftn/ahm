#ifndef THREAD_DATA_HPP
#define THREAD_DATA_HPP

#include "AdvancedHeapManager.hpp"

using namespace std;

struct ThreadData {
    AdvancedHeapManager* manager;
    int threadId;
};

DWORD WINAPI ThreadFunction(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    AdvancedHeapManager* ahm = data->manager;
    int threadId = data->threadId;

    const int NUM_ALLOCS = 5;
    void* ptrs[NUM_ALLOCS] = { nullptr };

    // Simulate allocations
    for (int i = 0; i < NUM_ALLOCS; i++) {
        size_t size = (rand() % 1000) + 1;
        ptrs[i] = ahm->Malloc(size);
        cout << "Thread " << threadId << " allocated " << size << " bytes at " << ptrs[i] << endl;
        Sleep(rand() % 1000);  // Simulate work
    }

    // Simulate deallocations
    for (int i = 0; i < NUM_ALLOCS; i++) {
        if (ptrs[i]) {
            ahm->Free(ptrs[i]);
            cout << "Thread " << threadId << " freed memory at " << ptrs[i] << endl;
        }
        Sleep(rand() % 1000);  // Simulate work
    }

    delete data;
    return 0;
}

#endif