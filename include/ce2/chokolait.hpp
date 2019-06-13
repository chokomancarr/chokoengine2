#pragma once

/* ChokoLait Interface for ChokoEngine (c) Chokomancarr
 *
 * See https://chokomancarr.github.io/ChokoLait/ for documentation and examples.
 */

#if !defined(PLATFORM_WIN) && !defined(PLATFORM_LNX) && !defined(PLATFORM_OSX)
#error("platform macro must be defined!")
#endif

#ifndef CHOKO_LAIT
#define CHOKO_LAIT
#endif

#define CHOKOLAIT_INIT_VARS(w, h) ChokoLait __chokolait_instance(w, h)

#include "chokoengine.hpp"


class ChokoLait {
	static bool initd;

	static ChokoEngine::Camera mainCamera;
public:
	ChokoLait(int w, int h);

    static bool alive();

    typedef void(*emptyCallbackFunc)(void);

    static void Update(emptyCallbackFunc func = 0);
    static void Paint(emptyCallbackFunc rendFunc = 0, emptyCallbackFunc paintFunc = 0);
};