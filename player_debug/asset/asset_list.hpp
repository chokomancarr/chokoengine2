#pragma once
#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

class EAssetList { CE_CLASS_COMMON
    struct _Entry {
        _Entry(const std::string& sig);

        std::string sig;
        Asset obj;
    };

    static std::array<std::vector<_Entry>, (int)AssetType::_COUNT> _entries;

public:
    static void Init();

    static const Asset& Get(AssetType t, const std::string& sig, bool async = false);
	static std::vector<std::string> GetList(AssetType t);
};

CE_END_PL_NAMESPACE