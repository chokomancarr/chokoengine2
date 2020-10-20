#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class ZLIB {
public:
    static std::vector<byte> Inflate(const std::vector<byte>&);
};

CE_END_NAMESPACE