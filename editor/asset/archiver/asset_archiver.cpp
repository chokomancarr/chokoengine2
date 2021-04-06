#include "chokoeditor.hpp"
#include "deflate.hpp"
#include "getdeps.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
    const size_t MAXLEN = 500000000; //500MB
}

void EAssetArchiver::Exec(const std::string& tar) {
	JsonObject list(JsonObject::Type::Group);

    int fi = 0;
    size_t fsz = 0;
    std::ofstream fl(tar + "assets" + std::to_string(fi) + ".dat", std::ios::binary);

	EDebug::Log("Asset Archive", "generating archives: 0");

    const auto& evs = EAssetList::entries();
    for (int a = 0; a < (int)AssetType::_COUNT; a++) {
        for (auto& e : evs[a]) {
			if (!AssetDepends::resultA[a].count(e.sig)) {
				continue;
			}
			std::cout << "writing " << e.sig << std::endl;
            const auto data = ZLIB::Deflate(
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
                fl.open(tar + "assets" + std::to_string(fi) + ".dat", std::ios::binary);

				EDebug::Log("Asset Archive", "generating archives: " + std::to_string(fi));
            }
			else {
				fl.write("\x00\xFF\x00\xFF", 4);
				fsz += 4;
			}
        }
    }

    std::ofstream(tar + "assets_index") << JsonParser::Export(list);

	EDebug::Log("Asset Archive", "archive generation completed");
};

CE_END_ED_NAMESPACE