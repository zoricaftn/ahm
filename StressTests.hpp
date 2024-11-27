#ifndef STRESS_TESTS_HPP
#define STRESS_TESTS_HPP

#include <windows.h>
#include <iostream>
#include "AdvancedHeapManager.hpp"

using namespace std;


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

   
}

#endif // !STRESS_TESTS_HPP