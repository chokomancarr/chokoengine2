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
SharedMemory<T>::SharedMemory() : _length(0), _data(nullptr) {}

template <typename T>
SharedMemory<T>::SharedMemory(const std::string& name, int n) : SharedMemory() {
    open(name, n);
}

template <typename T>
SharedMemory<T>::~SharedMemory() {
#ifdef PLATFORM_WIN

#else
    close();
#endif
}

template <typename T>
bool SharedMemory<T>::operator !() const {
#ifdef PLATFORM_WIN

#else
    return !_length || !_data;
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
volatile T& SharedMemory<T>::operator [](int i) const {
#ifdef PLATFORM_WIN

#else
    assert(i >= 0 && (i * sizeof(T)) < _length);
    return _data[i];
#endif
}

template <typename T>
void SharedMemory<T>::open(const std::string& name, int n) {
    close();

    _length = sizeof(T) * n;

#ifdef PLATFORM_WIN

#else
    auto fd = shm_open(("/" + name).c_str(), O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
    if (fd < 0) return;

    ftruncate(fd, _length);

    _data = (T*)mmap(0, _length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    ::close(fd);
#endif
}

template <typename T>
void SharedMemory<T>::close() {
#ifdef PLATFORM_WIN

#else
    if (_data) {
        munmap((void*)_data, _length);
    }
#endif

    _length = 0;
    _data = nullptr;
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
