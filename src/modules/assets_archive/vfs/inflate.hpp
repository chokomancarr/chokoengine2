#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class ZLIB {
public:
    static std::vector<unsigned char> Inflate(unsigned char* data, unsigned int len);
};

CE_END_NAMESPACE