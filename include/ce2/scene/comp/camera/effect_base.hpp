#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _CameraEffect {

public:
    /* The main apply effect function
     * This function must be defined
     * The output must be written to \p dst
     */
    virtual void Apply(const RenderTarget& src, const RenderTarget& dst, const RenderTarget& tmp, const FrameBuffer& gbuf) = 0;
};

CE_END_NAMESPACE