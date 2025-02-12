#pragma once
#include "chokoeditor.hpp"
#include "asset/enums/asset_type.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T>
void EO_SelectRef::ScanComps() {
	std::function<void(const SceneObject& o)> doscan = [&](const SceneObject& o) {
		const auto& c = o->GetComponent<T>();
		if (!!c) {
			_comps.push_back(c);
		}
		for (auto& o2 : o->children()) {
			doscan(o2);
		}
	};

	_comps.clear();
	for (auto& o : ChokoEditor::scene->objects()) {
		doscan(o);
	}
}

template <typename T>
void EO_SelectRef::RegAsset(T& slot, AssetType tp) {
	static_assert(std::is_base_of<_Asset, typename T::_TpBase>::value, "Target is not an asset!");
	auto i = GetInstance<EO_SelectRef>();
	i->_assetType = tp;
	i->state = std::unique_ptr<StateR<T>>(new StateR<T>(slot));
	i->state->type = _State::Type::Asset;
	i->_assets = EAssetList::GetList(i->_assetType);
	i->active = true;
}

template <typename T, typename F>
void EO_SelectRef::RegAsset(const T& slot, std::function<void(F)> setter, AssetType tp) {
	static_assert(std::is_base_of<_Asset, typename T::_TpBase>::value, "Target is not an asset!");
	auto i = GetInstance<EO_SelectRef>();
	i->_assetType = tp;
	i->state = std::unique_ptr<State<T, F>>(new State<T, F>(slot, setter));
	i->state->type = _State::Type::Asset;
	i->_assets = EAssetList::GetList(i->_assetType);
	i->active = true;
}

template <typename T>
void EO_SelectRef::RegComp(T& tar) {
	static_assert(std::is_base_of<_Component, typename T::_TpBase>::value, "Target is not a component!");
	auto i = GetInstance<EO_SelectRef>();
	i->state = std::unique_ptr<StateR<T>>(new StateR<T>(tar));
	i->state->type = _State::Type::Comp;
	i->ScanComps<T>();
	i->active = true;
}

template <typename T, typename F>
void EO_SelectRef::RegComp(const T& tar, std::function<void(F)> setter) {
	static_assert(std::is_base_of<_Component, typename T::_TpBase>::value, "Target is not a component!");
	auto i = GetInstance<EO_SelectRef>();
	i->state = std::unique_ptr<State<T, F>>(new State<T, F>(tar, setter));
	i->state->type = _State::Type::Comp;
	i->ScanComps<T>();
	i->active = true;
}

CE_END_ED_NAMESPACE
