#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EOverlay {
    static std::shared_ptr<EOverlay> instance;
protected:
    template <typename T>
    static T* GetInstance();
public:
    virtual ~EOverlay() = default;

    bool active;
    virtual void Update() {}
    virtual void Draw() = 0;
};

CE_END_ED_NAMESPACE

#include "detail/overlay.inl"
#include "overlay/eo_dropdown.hpp"
#include "overlay/eo_colorpicker.hpp"
#include "overlay/eo_selectref.hpp"
#include "overlay/eo_addcomp.hpp"