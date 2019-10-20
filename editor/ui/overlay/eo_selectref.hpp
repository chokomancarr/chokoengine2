#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_SelectRef : public EOverlay {
	Asset* _asset;
	Component* _comp;

	EAssetType _assetType;

	std::vector<std::string> _assets;
	std::vector<Component> _comps;

	template <typename T>
	void ScanComps();

public:
	void Draw() override;
	template <typename T>
	static void RegAsset(T& tar);
	template <typename T>
	static void RegComp(T& tar);
};

CE_END_ED_NAMESPACE

#include "eo_selectref.inl"