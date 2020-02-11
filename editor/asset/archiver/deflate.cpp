#include "chokoeditor.hpp"
#include "deflate.hpp"
#include <zlib.h>

CE_BEGIN_ED_NAMESPACE

std::vector<byte> ZLIB::Deflate(const std::vector<byte>& data) {
    return data;
}

CE_END_ED_NAMESPACE