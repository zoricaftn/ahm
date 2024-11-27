#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <windows.h>
#include <iostream>
#include <cstdlib>

template<typename T>
class Array {
private:
    T* data;
    size_t capacity;
    size_t length;

public:
    Array(size_t initialCapacity = 4) : capacity(initialCapacity), length(0) {
        data = new T[capacity];
    }

    ~Array() {
        delete[] data;
    }

    void add(const T& item) {
        if (length >= capacity) {
            size_t newCapacity = capacity * 2;
            T* newData = new T[newCapacity];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
        data[length++] = item;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    size_t size() const {
        return length;
    }
};

#endif // !ARRAY_HPP