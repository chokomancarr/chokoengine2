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

#define CHOKOLAIT_INIT_VARS ChokoLait __chokolait_instance;

#include "chokoengine.hpp"


class ChokoLait {
    static int initd;

    static void _InitVars();
public:
	static ChokoEngine::Camera mainCamera;

	ChokoLait();

    static void Init(int scrW, int scrH);

    static bool alive();

    typedef void(*emptyCallbackFunc)(void);

    static void Update(emptyCallbackFunc func = 0);
    static void Paint(emptyCallbackFunc rendFunc = 0, emptyCallbackFunc paintFunc = 0);
};