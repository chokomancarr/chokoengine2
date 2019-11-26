#pragma once

/* ChokoLait Interface for ChokoEngine (c) Chokomancarr
 *
 * See https://chokomancarr.github.io/ChokoLait/ for documentation and examples.
 */

#if !defined(PLATFORM_WIN) && !defined(PLATFORM_LNX) && !defined(PLATFORM_MAC)
#error("platform macro must be defined!")
#endif

#ifndef CHOKO_LAIT
#define CHOKO_LAIT
#endif

#include "chokoengine.hpp"

#include <functional>

CE_BEGIN_NAMESPACE

class ChokoLait {
    typedef void(*emptyCallbackFunc)(void);

	static bool initd;

	static CE_EXPORT Scene mainScene;
	static CE_EXPORT Camera mainCamera;
    
public:
    /* Initializes ChokoLait.
     * This function must be called before using any
     * ChokoLait or ChokoEngine API functions.
     */
	static bool Init(const std::string& title, int w, int h);

    /* Checks if the window is not closed
     * Can be used as the main loop condition
     */
    static bool alive();

    static void Update(emptyCallbackFunc func = 0);
    static void Paint(emptyCallbackFunc rendFunc = 0, emptyCallbackFunc paintFunc = 0);
};

CE_END_NAMESPACE
