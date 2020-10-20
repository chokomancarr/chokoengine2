#include "vfs.hpp"
#include "inflate.hpp"

CE_BEGIN_NAMESPACE

std::unordered_map<std::string, ArchiveParser::Entry> ArchiveParser::entries;
std::vector<std::ifstream> ArchiveParser::streams;

void ArchiveParser::Init(const std::string& path) {
	Debug::Message("ArchiveParser", "loading archive");
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
	Debug::Message("ArchiveParser", "loaded " + std::to_string(ms + 1) + " archives");
}

std::vector<byte> ArchiveParser::Read(const std::string& sig) {
    const auto& e = entries[sig];
    if (!e.len) return {};
    auto& strm = streams[e.fileIndex];
    strm.seekg(e.pos);
    strm.clear();
    std::vector<byte> data(e.len);
    strm.read((char*)data.data(), e.len);
    return ZLIB::Inflate(data);
}

std::string ArchiveParser::ReadStr(const std::string& sig) {
    const auto& dt = Read(sig);
    return std::string((char*)dt.data(), dt.size());
}

ArchiveParser::Strm ArchiveParser::GetStrm(const std::string& sig) {
	const auto& e = entries[sig];
	auto& strm = streams[e.fileIndex];
	strm.seekg(e.pos);
	strm.clear();
	return Strm(strm, e.len);
}

JsonObject ArchiveParser::GetMetaOf(const std::string& sig) {
    return entries[sig].meta;
}

CE_END_NAMESPACE