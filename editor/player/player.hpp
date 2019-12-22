#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EPlayer {
public:
    static bool playing;

    static Int2 targetReso;

    static Texture outputImage;

    static void Play();

    static void Sync();

    static void Stop();
};

CE_END_ED_NAMESPACE
