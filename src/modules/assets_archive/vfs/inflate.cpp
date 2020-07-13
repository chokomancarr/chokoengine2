#include "inflate.hpp"
#include <zlib.h>
#include <assert.h> 

CE_BEGIN_NAMESPACE

std::vector<unsigned char> ZLIB::Inflate(unsigned char* data, unsigned int len) {
    z_stream strm = {};
    if (inflateInit(&strm) != Z_OK) {
        return {};
    }
    strm.next_in = data;
    strm.avail_in = len;

    std::vector<unsigned char> res = {};
    std::array<unsigned char, 1024> buf;
    do {
        strm.next_out = buf.data();
        strm.avail_out = buf.size();
        auto ret = inflate(&strm, Z_NO_FLUSH);
        assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     /* and fall through */
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            inflateEnd(&strm);
            return res;
        }
        const auto have = 1024 - strm.avail_out;
        res.insert(res.end(), buf.begin(), buf.begin() + have);
    } while (strm.avail_out == 0);
    inflateEnd(&strm);
    return res;
}

CE_END_NAMESPACE