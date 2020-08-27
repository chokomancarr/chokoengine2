#include "chokoeditor.hpp"
#include "deflate.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
    const size_t MAXLEN = 500000000; //500MB

    std::vector<byte> GetAssetBytes(const AssetType tp, const std::string& sig) {
        return IO::ReadFileBinary(CE_DIR_ASSET + sig);
    }
}

void EAssetArchiver::Exec(const std::string& tar) {
    JsonObject list(JsonObject::Type::Group);

    int fi = 0;
    size_t fsz = 0;
    std::ofstream fl(tar + std::to_string(fi) + ".dat");

	EDebug::Log("Asset Archive", "generating archives: 0");

    const auto& evs = EAssetList::entries();
    for (int a = 0; a < (int)AssetType::_COUNT; a++) {
        for (auto& e : evs[a]) {
            const auto data = ZLIB::Deflate(
                //GetAssetBytes((AssetType)a, e.sig)
				IO::ReadFileBinary(CE_DIR_ASSET + e.sig)
            );
            const auto dsz = data.size();

            list.group.push_back(JsonPair(e.sig, JsonObject({
                JsonPair(JsonObject("fileIndex"), std::to_string(fi)),
                JsonPair(JsonObject("offset"), std::to_string(fsz)),
                JsonPair(JsonObject("length"), std::to_string(dsz)),
                JsonPair(JsonObject("meta"), JsonParser::Parse(
                    IO::ReadFile(CE_DIR_ASSET + e.sig + ".meta")))
            })));

            fl.write((char*)data.data(), dsz);
            if ((fsz += dsz) >= MAXLEN) {
                fi++;
                fsz = 0;
                fl.close();
                fl.open(tar + std::to_string(fi) + ".dat");

				EDebug::Log("Asset Archive", "generating archives: " + std::to_string(fi));
            }
        }
    }

    std::ofstream(tar + "_index.json") << JsonParser::Export(list);

	EDebug::Log("Asset Archive", "archive generation completed");
};

CE_END_ED_NAMESPACE