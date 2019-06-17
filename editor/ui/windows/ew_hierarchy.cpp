#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_Hierarchy::DrawMenuObject(int& off, const std::vector<SceneObject>& oo, int level) {

}

void EW_Hierarchy::DrawMenu() {
    int off = 0;
    DrawMenuObject(off, ChokoLait::scene()->objects(), 0);
}

CE_END_ED_NAMESPACE