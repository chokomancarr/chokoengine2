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
SharedMemory<T>::SharedMemory() : _length(0) {}

template <typename T>
SharedMemory<T>::SharedMemory(const std::string& name, int n) : _length(sizeof(T) * n), _data(nullptr) {
#ifdef PLATFORM_WIN

#else
    shm_file = shm_open(name.c_str(), O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
    if (shm_file < 0) return;

    ftruncate(shm_file, _length);

    _data = (T*)mmap(0, _length, PROT_READ | PROT_WRITE, MAP_SHARED, shm_file, 0);
#endif
}

template <typename T>
SharedMemory<T>::~SharedMemory() {
#ifdef PLATFORM_WIN

#else
    if (_data) {
        munmap((void*)_data, _length);
        close(shm_file);
    }
#endif
}

template <typename T>
bool SharedMemory<T>::operator !() const {
#ifdef PLATFORM_WIN

#else
    return _length > 0 && !!_data;
#endif
}

template <typename T>
volatile T* SharedMemory<T>::operator ->() const {
#ifdef PLATFORM_WIN

#else
    return _data;
#endif
}

template <typename T>
T& SharedMemory<T>::operator [](int i) const {
#ifdef PLATFORM_WIN

#else
    assert(i >= 0 && (i * sizeof(T)) < _length);
    return _data[i];
#endif
}

template <typename T>
volatile T* SharedMemory<T>::data() const {
#ifdef PLATFORM_WIN

#else
    return _data;
#endif
}

template <typename T>
size_t SharedMemory<T>::length() const {
    return _length / sizeof(T);
}

CE_END_NAMESPACE
