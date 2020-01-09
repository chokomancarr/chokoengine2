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
SharedMemory<T>::SharedMemory() : _length(0)
#ifdef PLATFORM_WIN
	, _handle(nullptr)
#endif
	, _data(nullptr) {}

template <typename T>
SharedMemory<T>::SharedMemory(const std::string& name, int n) : SharedMemory() {
	open(name, n);
}

template <typename T>
SharedMemory<T>::~SharedMemory() {
    close();
}

template <typename T>
bool SharedMemory<T>::operator !() const {
    return !_length || !_data;
}

template <typename T>
volatile T* SharedMemory<T>::operator ->() const {
    return _data;
}

template <typename T>
volatile T& SharedMemory<T>::operator [](int i) const {
    assert(i >= 0 && (i * sizeof(T)) < _length);
    return _data[i];
}

template <typename T>
void SharedMemory<T>::open(const std::string& name, int n) {
    close();

    _length = sizeof(T) * n;

#ifdef PLATFORM_WIN
	_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, _length, name.c_str());
	if (!_handle) return;

	_data = (T*)MapViewOfFile(_handle, FILE_MAP_ALL_ACCESS, 0, 0, _length);
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
	if (_data) {
#ifdef PLATFORM_WIN
		UnmapViewOfFile((LPCVOID)_data);
		CloseHandle(_handle);
#else
        munmap((void*)_data, _length);
#endif
		_data = nullptr;
	}
    _length = 0;
}

template <typename T>
volatile T* SharedMemory<T>::data() const {
    return _data;
}

template <typename T>
size_t SharedMemory<T>::length() const {
    return _length / sizeof(T);
}

template <typename T>
void SharedMemory<T>::flush() const {
	if (_data) {
#ifdef PLATFORM_WIN
		FlushViewOfFile((void*)_data, _length);
		FlushFileBuffers(_handle);
#else
		msync((void*)_data, _length, MS_SYNC);
#endif
	}
}

CE_END_NAMESPACE
