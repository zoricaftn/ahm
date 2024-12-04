#ifndef STRESS_TESTS_HPP
#define STRESS_TESTS_HPP

#include <windows.h>
#include <iostream>
#include "AdvancedHeapManager.hpp"
#include "ThreadData.hpp"

using namespace std;

DWORD WINAPI ThreadFunction(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    AdvancedHeapManager* ahm = data->
manager;
    int threadId = data->threadId;

    const int NUM_ALLOCS = 5;
    void* ptrs[NUM_ALLOCS] = { nullptr };

    // Allocations
    for (int i = 0; i < NUM_ALLOCS; i++) {
        size_t size = (rand() % 1000) + 1;
        ptrs[i] = ahm->Malloc(size);
        cout << "Thread " << threadId << " allocated " << size << " bytes at " << ptrs[i] << endl;
        Sleep(rand() % 1000);
    }

    // Deallocations
    for (int i = 0; i < NUM_ALLOCS; i++) {
        if (ptrs[i]) {
            ahm->Free(ptrs[i]);
            cout << "Thread " << threadId << " freed memory at " << ptrs[i] << endl;
        }
        Sleep(rand() % 1000);
    }

    delete data;
    return 0;
}

void RunStressTests() {
    const int NUM_THREADS = 8;
    AdvancedHeapManager ahm(12);  // 4 heaps

    std::cout << "Starting stress tests...\n";

    // Test 1: Rapid Allocation/Deallocation Cycles
    {
        AdvancedHeapManager ahm(30);
        HANDLE threads[NUM_THREADS] = { NULL };

        // Create threads
        for (int i = 0; i < NUM_THREADS; i++) {
            ThreadData* data = new ThreadData{ &ahm, i };
            threads[i] = CreateThread(nullptr, 0, ThreadFunction, data, 0, nullptr);
        }

        // Wait for all threads to complete
        WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

        // Close thread handles
        for (int i = 0; i < NUM_THREADS; i++) {
            if (threads[i] != NULL)
                CloseHandle(threads[i]);
        }

        // Print final statistics
        ahm.PrintStats();
    }

    // Test 2: Mixed Size Operations
    {
        AdvancedHeapManager ahm(20);
        HANDLE threads[NUM_THREADS] = { NULL };

        // Create threads
        for (int i = 0; i < NUM_THREADS; i++) {
            ThreadData* data = new ThreadData{ &ahm, i + 30 };
            threads[i] = CreateThread(nullptr, 0, ThreadFunction, data, 0, nullptr);
        }

        // Wait for all threads to complete
        WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

        // Close thread handles
        for (int i = 0; i < NUM_THREADS; i++) {
            if (threads[i] != NULL)
                CloseHandle(threads[i]);
        }

        // Print final statistics
        ahm.PrintStats();
    }
}

#endif // !STRESS_TESTS_HPP