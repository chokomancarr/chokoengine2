#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class ArchiveParser {
public:
    struct Entry {
        int fileIndex;
        size_t pos;
        size_t len;
        JsonObject meta;
    };
    static std::unordered_map<std::string, Entry> entries;
    static std::vector<std::ifstream> streams;

    // /dir/to/file [0/1/2/... .dat]
    static void Init(const std::string& path);

    static std::vector<byte> Read(const std::string& sig);
    static std::string ReadStr(const std::string& sig);

    static JsonObject GetMetaOf(const std::string& sig);
};

CE_END_NAMESPACE