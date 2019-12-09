ChokoEngine
==================================

Definition
----------

.. class:: ChokoEngine

Typedefs
------------------

.. type:: uint64_t objectid

    Unique identifier for every instanced object. This value is guaranteed unique for the lifetime of the engine instance.

Method functions
----------------

.. Note:: If you are using :expr:`ChokoLait` (you should!), do NOT call these functions directly.

.. function:: static bool Init()

    Initializes the engine, and all its components

.. function:: static void Cleanup()

    Clean up all resources, and shuts down the engine