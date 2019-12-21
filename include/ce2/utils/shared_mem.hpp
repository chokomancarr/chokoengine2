#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
class SharedMemory { CE_CLASS_COMMON
    size_t _length;

#ifdef PLATFORM_WIN

#else
    volatile T* _data;
#endif

public:
    SharedMemory();
    SharedMemory(const std::string& name, int n = 1);
    SharedMemory(const SharedMemory<T>&) = delete;

    ~SharedMemory();

    bool operator !() const;

    volatile T* operator ->() const;
    
    volatile T& operator [](int) const;

    void open(const std::string& name, int n = 1);

    void close();

    volatile T* data() const;

    size_t length() const;
};

CE_END_NAMESPACE

#include "detail/shared_mem.inl"
