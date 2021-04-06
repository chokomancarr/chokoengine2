#include "../../getdeps.hpp"

CE_BEGIN_ED_NAMESPACE

CE_DP_BEGIN_IMPL_A(Prefab)

const auto sig = obj->assetSignature();
obj->assetSignature("<instantiated>");
auto res = obj->Instantiate([](AssetType t, const std::string& s) -> Asset {
	return EAssetList::Get(t, s, true);
});
obj->assetSignature(sig);
Reg(res);

CE_DP_END_IMPL_A()

CE_END_ED_NAMESPACE