#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_I_Status::UMap::Add(ChokoEngine::objectid i, const pEW_I_Status& s) {
    _umap[i] = s;
}

void EW_I_Status::UMap::Erase(ChokoEngine::objectid i) {
    _umap.erase(i);
}

CE_END_ED_NAMESPACE