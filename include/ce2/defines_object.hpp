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
	friend ::ChokoEngine::Backend::Renderer;\
	friend ::ChokoEngine::Backend::GI;
#define CE_OBJECT_COMMON\
	CE_CLASS_COMMON\
	public:\
    static size_t _object_id() { return __COUNTER__; }\
	private:
#define CE_COMPONENT_COMMON\
	CE_OBJECT_COMMON\
	protected:\
	Component Clone() const override {\
		typedef std::remove_cv<std::remove_reference<decltype(*this)>::type>::type _BaseTp;\
	    return Ref<_BaseTp>::New(*this);\
	}\
	private:

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

/* Objects that can be loaded in the background
 * LoadAsync setting _loading = false must be implemented
 */
#define CE_OBJECT_ALLOW_ASYNC\
	public:\
	bool loaded();\
	private:\
	std::thread _asyncThread;\
	std::mutex _asyncLock;\
	byte _asyncLoadStatus = 0;\
	void LoadAsync(); /*<-Implement this function*/\
	byte _GetAsyncStatus() {\
		if (!_loading) return 0;\
		std::lock_guard<std::mutex> lock(_asyncLock);\
		return _asyncLoadStatus;\
	}\
	void _SetAsyncStatus(byte b) {\
		std::lock_guard<std::mutex> lock(_asyncLock);\
		_asyncLoadStatus = b;\
	}

#define CE_OBJECT_INIT_ASYNC\
	if (!async) {\
		_asyncThread.join();\
		LoadAsync();\
	}\
	else _loading = true;

#define CE_OBJECT_SET_ASYNC_LOADING\
	_SetAsyncStatus(1)

#define CE_OBJECT_SET_ASYNC_READY\
	_SetAsyncStatus(2)

#define CE_OBJECT_ASYNC_READY\
	(_GetAsyncStatus() == 2)

#define CE_OBJECT_CHECK_ASYNC\
	if (CE_OBJECT_ASYNC_READY) {\
		LoadAsync();\
	}

#define CE_OBJECT_FINALIZE_ASYNC\
	if (_asyncThread.joinable()) _asyncThread.join();
