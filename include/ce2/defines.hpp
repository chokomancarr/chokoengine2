#pragma once

#define CE_VERSION_MAJOR 2
#define CE_VERSION_MINOR 0
#define CE_VERSION_REVISION 0

#define BEGIN_CE_NAMESPACE namespace ChokoEngine {
#define END_CE_NAMESPACE }

#define GET_MEMBER_FUNC(tp, nm) static tp nm() { return _ ## nm; }
#define SET_MEMBER_FUNC(tp, nm) static void nm(const tp& v) { _ ## nm = v; }