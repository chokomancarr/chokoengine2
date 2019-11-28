/* This is a minimal example for ChokoLait
 * This should create a black window with the title "Hello World"
 */

/* Include this file instead of chokoengine.hpp
 * to use shorthand functions
 */
#include "chokolait.hpp"

/* All ChokoEngine fucntions are contained in
 * the ChokoEngine namespace
 */
using namespace ChokoEngine;

int main() {
	/* Initializes ChokoLait
	 * This must be called first
	 * before other ChokoEngine operations
	 */
	if (!ChokoLait::Init(
		/* Title of the window
		 */
		"Hello World",
		/* Width of the window
		 */
		800,
		/* Height of the window
		 */
		600)) {

		/* Initialization failed, we're done
		 */
		return -1;
	}

	/* While the window is not closed
	 */
	while (ChokoLait::alive()) {
		/* Updates the engine state
		 * All frame-dependant calls should come after this function
		 */
		ChokoLait::Update(
			/* optional function to call before updating the scene
			 */
			nullptr
		);
		/* Redraws the window
		 */
		ChokoLait::Paint(
			/* optional function to draw into the camera target
			 */
			nullptr,
			/* optional function to draw onto the screen
			 */
			nullptr
		);
	}
}