#pragma once
#include "chokoeditor.hpp"
#include "enums/asset_type.hpp"
#include "enums/export_type.hpp"
#include "enums/other_type.hpp"

CE_BEGIN_ED_NAMESPACE

class EAssetList { CE_CLASS_COMMON
    struct _Entry {
        _Entry(const std::string& sig, const std::string& gsig = "");

        std::string sig;
		std::string genSig;
        Asset obj;
        time_t modtime;
    };
	struct _ScriptEntry {
		_ScriptEntry(const std::string& sig);

		std::string sig;
		ScriptInfo info;
		time_t modtime;
		bool changed; //need to regen loader
	};
    /* List of all asset data
     */
    static std::array<std::vector<_Entry>, (int)AssetType::_COUNT> _entries;
	static std::array<std::vector<_Entry>, (int)EExportType::_COUNT> _exportEntries;
	static std::array<std::vector<_Entry>, (int)EExtType::_COUNT> _otherEntries;
	static std::vector<_ScriptEntry> _scriptEntries;

    static bool Scan_Fd(const std::string& fd);

public:
    struct TypeOfSt {
		enum class Type {
			Asset,
			Export,
			Other,
			Unknown
		} subtype;
		AssetType assetType;
		EExportType exportType;
		EExtType otherType;
	};

    static void Init();
    
    static void Rescan();

    static const Asset& Get(AssetType t, const std::string& sig, bool async = false);
	static std::vector<std::string> GetList(AssetType t);
	static std::vector<std::string> GetScrList();
	static const ScriptInfo& GetScr(const std::string& sig);

	static Asset Get2(AssetType, const std::string&);

	CE_GET_ST_MEMBER(entries);
	CE_GET_ST_MEMBER(scriptEntries);

	static TypeOfSt TypeOf(const std::string& f);
};

CE_END_ED_NAMESPACE