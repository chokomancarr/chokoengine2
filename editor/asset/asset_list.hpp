#pragma once
#include "chokoeditor.hpp"
#include "enums/asset_type.hpp"
#include "enums/export_type.hpp"

CE_BEGIN_ED_NAMESPACE

class EAssetList { CE_CLASS_COMMON
    struct _Entry {
        _Entry(const std::string& sig);

        std::string sig;
        Asset obj;
        time_t modtime;
    };
	struct _ScriptEntry {
		_ScriptEntry(const std::string& sig);

		std::string sig;
		ScriptInfo info;
		time_t modtime;
	};
    /* List of all asset data
     */
    static std::array<std::vector<_Entry>, (int)EAssetType::_COUNT> _entries;
    static std::array<std::vector<std::string>, (int)EAssetType::_COUNT> _exts;
    /* These files are not assets directly, but will generate them
     */
    static std::array<std::vector<std::string>, (int)EExportType::_COUNT> _export_exts;
	static std::vector<_ScriptEntry> _scriptEntries;

    static bool Scan_Fd(const std::string& fd);

    static void UpdateModTime(const std::string& fl, bool now);

public:
    struct TypeOfSt {
		bool exported;
		EAssetType assetType;
		EExportType exportType;
	};

    static void Init();
    
    static void Rescan();

    static const Asset& Get(EAssetType t, const std::string& sig, bool async = false);

	static TypeOfSt TypeOf(const std::string& f);
};

CE_END_ED_NAMESPACE