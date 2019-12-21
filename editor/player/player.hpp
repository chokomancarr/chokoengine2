#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EPlayer {
public:
    static Int2 targetReso;

    static void Play();

    static void Sync();

    static void Stop();
};

CE_END_ED_NAMESPACE
