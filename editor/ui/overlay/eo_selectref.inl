#pragma once
#include "chokoeditor.hpp"
#include "asset/enums/asset_type.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T>
void EO_SelectRef::ScanComps() {
	std::function<void(const SceneObject& o)> doscan = [&](const SceneObject& o) {
		const auto& c = o->GetComponent<T>();
		if (!!c) {
			_comps.push_back((Component)c);
		}
		for (auto& o2 : o->children()) {
			doscan(o2);
		}
	};

	_comps.clear();
	for (auto& o : Scene::objects()) {
		doscan(o);
	}
}

template <typename T>
void EO_SelectRef::RegAsset(T& tar) {
	static_assert(std::is_base_of<_Asset, typename T::_TpBase>::value, "Target is not an asset!");
	auto i = GetInstance<EO_SelectRef>();
	i->_comp = nullptr;
	i->_asset = (Asset*)&tar;
	i->_assetType = EAssetTypeOf<T>::value;
	i->_assets = EAssetList::GetList(i->_assetType);
	i->active = true;
}

template <typename T>
void EO_SelectRef::RegComp(T& tar) {
	static_assert(std::is_base_of<_Component, typename T::_TpBase>::value, "Target is not a component!");
	auto i = GetInstance<EO_SelectRef>();
	i->_asset = nullptr;
	i->_comp = (Component*)&tar;
	i->ScanComps<T>();
	i->active = true;
}

CE_END_ED_NAMESPACE