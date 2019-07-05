#pragma once
#include "chokoeditor.hpp"
#include "enums/asset_type.hpp"

CE_BEGIN_ED_NAMESPACE

class EAssetList { CE_CLASS_COMMON
    struct _Entry {
        _Entry(const std::string& sig);

        std::string sig;
        Asset obj;
        time_t modtime;
    };
    /* List of all asset data
     */
    static std::array<std::vector<_Entry>, (int)EAssetType::_COUNT> _entries;
    static std::array<std::vector<std::string>, (int)EAssetType::_COUNT> _exts;
    /* These files are not assets directly, but will probably generate stuff
     */
    

    static bool Scan_Fd(const std::string& fd);

public:
    static void Init();
    
    static void Rescan();

    static const Asset& Get(EAssetType t, const std::string& sig);
};

CE_END_ED_NAMESPACE