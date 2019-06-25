#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

struct TextureOptions {
    /* Horizontal wrap convention
     */
    TextureWrap xwrap;
    /* Vertical wrap convention
     */
    TextureWrap ywrap;
    /* Levels of mipmaps to generate
     */
    int mipmaps;
    /* Scaling convention (false = nearest)
     */
    bool linear;

    TextureOptions() :
        xwrap(TextureWrap::Clamp),
        ywrap(TextureWrap::Clamp),
        mipmaps(5),
        linear(true)
    {}
    
    TextureOptions(TextureWrap xwrap, TextureWrap ywrap, int mips, bool linear) :
        xwrap(xwrap), ywrap(ywrap), mipmaps(mips), linear(linear) {}
};

CE_END_NAMESPACE