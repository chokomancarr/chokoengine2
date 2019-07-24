#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T>
std::shared_ptr<T> EW_I_Status::UMap::Get(ChokoEngine::objectid i) {
    static_assert(std::is_base_of<EW_I_Status, T>::value, "UMap::Get called with T which is not a status class!");
    auto& p = _umap[i];
    if (!p) {
        const auto tp = std::make_shared<T>();
        p = tp;
        return tp;
    }
	else return std::static_pointer_cast<T>(p);
}

CE_END_ED_NAMESPACE