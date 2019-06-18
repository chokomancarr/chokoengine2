#pragma once

#define CE_CLASS_COMMON\
	friend ChokoEngine::Engine;\
	friend ChokoEngine::ChokoLait;\
	friend ChokoEngine::Backend::Renderer;

#define CE_OBJECT_COMMON\
	CE_CLASS_COMMON\

#define CE_COMPONENT_COMMON\
	CE_OBJECT_COMMON

#define CE_OBJECT(nm)\
	class _ ## nm;\
	typedef Ref<_ ## nm> nm;\
	typedef std::weak_ptr<_ ## nm> p ## nm;

#define CE_GET_MEMBER(nm) const decltype(_ ## nm)& nm() const { return _ ## nm; }
#define CE_SET_MEMBER(nm) void nm(const decltype(_ ## nm)& v) { _ ## nm = v; }
#define CE_GET_SET_MEMBER(nm)\
    CE_GET_MEMBER(nm)\
    CE_SET_MEMBER(nm)
#define CE_SET_MEMBER_F(nm) void nm(const decltype(_ ## nm)& v);

#define CE_GET_ST_MEMBER(nm) static const decltype(_ ## nm)& nm() { return _ ## nm; }
#define CE_SET_ST_MEMBER(nm) static void nm(const decltype(_ ## nm)& v) { _ ## nm = v; }
#define CE_GET_SET_ST_MEMBER(nm)\
    CE_GET_ST_MEMBER(nm)\
    CE_SET_ST_MEMBER(nm)