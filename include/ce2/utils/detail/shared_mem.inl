#pragma once
#include "chokoengine.hpp"
#ifdef PLATFORM_WIN

#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

CE_BEGIN_NAMESPACE

template <typename T>
SharedMemory<T>::SharedMemory() : length(0) {}

template <typename T>
SharedMemory<T>::SharedMemory(const std::string& name, int n) : length(sizeof(T) * n), data(nullptr) {
#ifdef PLATFORM_WIN

#else
    shm_file = shm_open(name.c_str(), O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
    if (shm_file < 0) return;

    ftruncate(shm_file, length);

    data = mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, shm_file, 0);
#endif
}

template <typename T>
SharedMemory<T>::~SharedMemory() {
#ifdef PLATFORM_WIN

#else
    if (data) {
        munmap(data, length);
        close(shm_file);
    }
#endif
}

template <typename T>
bool SharedMemory<T>::operator !() const {
#ifdef PLATFORM_WIN

#else
    return length > 0 && !!data;
#endif
}

template <typename T>
T& SharedMemory<T>::operator ->() const {
#ifdef PLATFORM_WIN

#else
    return *data;
#endif
}

template <typename T>
T& SharedMemory<T>::operator [](int i) const {
#ifdef PLATFORM_WIN

#else
    assert(i >= 0 && (i * sizeof(T)) < length);
    return data[i];
#endif
}

template <typename T>
volatile T* SharedMemory<T>::data() const {
#ifdef PLATFORM_WIN

#else
    return data;
#endif
}

template <typename T>
size_t SharedMemory<T>::length() const {
    return length / sizeof(T);
}

CE_END_NAMESPACE
