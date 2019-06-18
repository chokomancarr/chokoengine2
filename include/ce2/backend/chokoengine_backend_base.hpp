#pragma once
#include "chokoengine.hpp"

#define CE_BEGIN_BK_NAMESPACE CE_BEGIN_NAMESPACE\
    namespace Backend {

#define CE_END_BK_NAMESPACE }\
    CE_END_NAMESPACE

#include "rend/renderer_base.hpp"