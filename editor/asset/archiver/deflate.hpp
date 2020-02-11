#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ZLIB {
public:
    static std::vector<byte> Deflate(const std::vector<byte>&);
};

CE_END_ED_NAMESPACE