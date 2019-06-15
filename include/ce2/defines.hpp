#pragma once

#define CE_VERSION_MAJOR 2
#define CE_VERSION_MINOR 0
#define CE_VERSION_REVISION 0

#define CE_NAMESPACE ChokoEngine
#define CE_NS ::CE_NAMESPACE::

#define CE_BEGIN_NAMESPACE namespace CE_NAMESPACE {
#define CE_END_NAMESPACE }

#define CE_NOT_IMPLEMENTED\
    throw std::logic_error("Function not implemented!");

#include "defines_object.hpp"