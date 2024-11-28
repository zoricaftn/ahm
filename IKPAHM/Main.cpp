#include "../Common/ThreadData.hpp"
#include "../Common/AdvancedHeapManager.hpp"
#include "../Common/StressTests.hpp"

#define NUM_THREADS 8

int main() {
    int choice = 0;
    cout << "1. Run stress tests" << endl << "2. Run program" << endl;
    cin >> choice;

    if (choice == 1)
    {
        RunStressTests();
    }
    else
    {
        int number_of_heaps = 0;

        cout << "Enter number of heaps for AHM: ";
        cin >> number_of_heaps;

        AdvancedHeapManager ahm(number_of_heaps);
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

    return 0;
}