#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class AssetDepends {
public:
	static std::array<std::unordered_set<
		std::string>, (size_t)AssetType::_COUNT> resultA;
	static std::array<std::unordered_set<
		std::string>, (size_t)EExtType::_COUNT> resultE;

	static void Clear();

	static void Reg(const SceneObject&);

#define CS(c) static void Reg ## c(const c&);

#include "ce2/asset/detail/assetdefsall.inl"
	ASSET_DEFS_ALL(CS)
#include "ce2/asset/detail/assetdefsall_undef.inl"

#include "ce2/scene/comp/detail/compdefsall.inl"
	COMP_DEFS_ALL(CS)
#include "ce2/scene/comp/detail/compdefsall_undef.inl"

#undef CS

};

CE_END_ED_NAMESPACE

#define CE_DP_BEGIN_IMPL_A(tp)\
	void AssetDepends::Reg ## tp(const tp& obj) {\
		if (!obj) return;\
		std::cout << "   -- reg " << obj->assetSignature() << std::endl;\
		resultA[(size_t)AssetType::tp].emplace(\
			obj->assetSignature());
		//if (!resultA[(size_t)AssetType::tp].emplace(\
		//	obj->assetSignature()).second) return;

#define CE_DP_END_IMPL_A() }

#define CE_DP_BEGIN_IMPL_C(tp)\
	void AssetDepends::Reg ## tp(const tp& obj) {

#define CE_DP_END_IMPL_C() }