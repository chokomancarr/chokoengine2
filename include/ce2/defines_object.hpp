#pragma once

#define CE_OBJECT(nm)\
	class _ ## nm;\
	typedef Ref<_ ## nm> nm;\
	typedef Ref_w<_ ## nm> p ## nm;

/* The _COMMON tag must be appended at the start of every class
 * according to their type
 * - class: static singletons
 * - object: instances declared with CE_OBJECT
 * - component: components inheriting _Component
 */
#define CE_CLASS_COMMON\
	friend ::ChokoEngine::ChokoEngine;\
	friend ::ChokoEngine::ChokoLait;\
	friend ::ChokoEngine::Backend::Renderer;
#define CE_OBJECT_COMMON\
	CE_CLASS_COMMON\
	public:\
    static size_t _object_id() { return __COUNTER__; }\
	private:
#define CE_COMPONENT_COMMON\
	CE_OBJECT_COMMON

/* Handy macros to add getters and setters
 */
#define CE_GET_MEMBER_MUT(nm) decltype(_ ## nm)& nm() { return _ ## nm; }
#define CE_GET_MEMBER(nm) const decltype(_ ## nm)& nm() const { return _ ## nm; }
#define CE_SET_MEMBER(nm) void nm(const decltype(_ ## nm)& v) { _ ## nm = v; }
#define CE_GET_SET_MEMBER_MUT(nm)\
    CE_GET_MEMBER_MUT(nm)\
    CE_SET_MEMBER(nm)
#define CE_GET_SET_MEMBER(nm)\
    CE_GET_MEMBER(nm)\
    CE_SET_MEMBER(nm)
#define CE_SET_MEMBER_F(nm) void nm(const decltype(_ ## nm)& v);
#define CE_GET_SET_MEMBER_F(nm)\
    CE_GET_MEMBER(nm)\
    CE_SET_MEMBER_F(nm)

#define CE_GET_ST_MEMBER(nm) static const decltype(_ ## nm)& nm() { return _ ## nm; }
#define CE_SET_ST_MEMBER(nm) static void nm(const decltype(_ ## nm)& v) { _ ## nm = v; }
#define CE_GET_SET_ST_MEMBER(nm)\
    CE_GET_ST_MEMBER(nm)\
    CE_SET_ST_MEMBER(nm)
