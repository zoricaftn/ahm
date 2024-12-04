#ifndef THREAD_DATA_HPP
#define THREAD_DATA_HPP

#include "AdvancedHeapManager.hpp"

using namespace std;

struct ThreadData {
    AdvancedHeapManager* manager;
    int threadId;
};
#endif