ChokoLait
================

Single-header library for ChokoEngine. You can integrate ChokoLait into your app by including `ChokoLait.hpp` and linking with ChokoEngine in cmake.

Definition
----------

.. class:: ChokoLait

Typedefs
------------------

.. type:: void(*ChokoLait::emptyCallbackFunc)(void)

    An zero-argument no-return-value function, for callbacks

Method functions
----------------

.. function:: static bool ChokoLait::Init(int w, int h, const InitOptionsSt& opts = InitOptionsSt())

    Initializes ChokoEngine. This should be called first before any other library usage.

.. function:: static bool ChokoLait::alive()

    Returns `true` if the application window is still available.

.. function:: static void ChokoLait::Update(emptyCallbackFunc func = 0)

    Updates the engine state. This function increments the game timer, so all frame-dependant calls should come after this function. :expr:`func` specifies an optional function to call before the scene is updated. 

.. function:: static void ChokoLait::Paint(emptyCallbackFunc rendFunc = 0, emptyCallbackFunc paintFunc = 0)

    Draws to the application window. :expr:`rendFunc` specifies an optional function to draw to the target camera buffer, while :expr:`paintFunc` specifies an optional function to draw directly onto the screen. Drawing calls outside this function have no effect.

Child objects
--------------

.. struct:: ChokoLait::InitOptionsSt

    Specifies optional arguments for the application initialization

Variables
---------

.. var:: std::string ChokoLait::InitOptionsSt::title = "ChokoLait App"

    Title of the window

.. var:: bool ChokoLait::InitOptionsSt::visible = true

    Is the window visible. Set this to :expr:`false` for off-target rendering