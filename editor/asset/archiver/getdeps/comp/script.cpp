#include "../../getdeps.hpp"
#include "ce2/asset/detail/assetdefsall.inl"
#include "ce2/scene/comp/detail/compdefsall.inl"

CE_BEGIN_ED_NAMESPACE

namespace {
	void regval(const ScriptInfo& info, const ScriptVar& vr, const ScriptVal& vl) {
		if (vr.is_vector) {
			int len = (int)vl.val_vec.size();
			for (auto& vl2 : vl.val_vec) {
				regval(info, vl2.var, vl2);
			}
			return;
		}
		switch (vr.type) {
		case ScriptVar::Type::Asset: {
#define CS(c) case AssetType::c: AssetDepends::Reg ## c((c)vl.val_asset); break;
			switch (vl.var.type_asset) {
				ASSET_DEFS_ALL(CS)
			}
#undef CS
			break;
		}
		case ScriptVar::Type::Component: {
#define CS(c) case ComponentType::c: AssetDepends::Reg ## c((c)vl.val_comp); break;
			switch (vr.type_comp) {
				COMP_DEFS_ALL(CS)
			}
#undef CS
			break;
		}
		case ScriptVar::Type::Class: {
			for (auto& sc : info->classes) {
				if (sc.name == vl.var.sub_class) {
					for (size_t a = 0; a < sc.vars.size(); a++) {
						regval(info, sc.vars[a], vl.val_class.val[a]);
					}
					break;
				}
			}
			break;
		}
		default:
			break;
		}
	}
}

CE_DP_BEGIN_IMPL_C(Script)

const auto scr = (DummyScript)obj;
const auto& _pvl = scr->info();
const auto& _vl = !_pvl ? nullptr : _pvl.lock();
for (auto& vl : scr->vals) {
	regval(_vl, vl.var, vl);
}

CE_DP_END_IMPL_C()

CE_END_ED_NAMESPACE