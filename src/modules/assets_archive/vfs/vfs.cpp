#include "vfs.hpp"
#include "inflate.hpp"

CE_BEGIN_NAMESPACE

void ArchiveParser::Init(const std::string& path) {
    int ms = 0;
    JsonObject json = JsonParser::Parse(IO::ReadFile(path + "_index.json"));
    for (auto& j : json.group) {
        Entry e = {};
        auto& jv = j.value;
        e.fileIndex = jv.Get("fileIndex").ToInt();
        e.pos = jv.Get("offset").ToInt();
        e.len = jv.Get("length").ToInt();
        e.meta = jv.Get("meta");
        entries.insert(std::make_pair(j.key.string, e));
        ms = std::max(ms, e.fileIndex);
    }
    streams.resize(ms + 1);
    for (int a = 0; a <= ms; a++) {
        streams[a].open(path + std::to_string(a) + ".dat", std::ios::binary);
    }
}

std::vector<byte> ArchiveParser::Read(const std::string& sig) {
    const auto& e = entries[sig];
    if (!e.len) return {};
    auto& strm = streams[e.fileIndex];
    strm.seekg(e.pos);
    strm.clear();
    std::vector<char> data(e.len);
    strm.read(data.data(), e.len);
    return ZLIB::Inflate((byte*)data.data(), e.len);
}

std::string ArchiveParser::ReadStr(const std::string& sig) {
    const auto& dt = Read(sig);
    return std::string((char*)dt.data(), dt.size());
}

JsonObject ArchiveParser::GetMetaOf(const std::string& sig) {
    return entries[sig].meta;
}

CE_END_NAMESPACE