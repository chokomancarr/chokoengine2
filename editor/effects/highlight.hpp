#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_OBJECT(HighlightEffect);

class _HighlightEffect : public CE_NS _CameraEffect {
public:
    Material material;

    void Apply(const RenderTarget& src, const RenderTarget& dst, const RenderTarget& tmp, const FrameBuffer& gbuf) override;
};

CE_END_ED_NAMESPACE