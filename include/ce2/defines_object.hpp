#pragma once

#define CE_CLASS_COMMON\
	friend ChokoEngine::Engine;\
	friend ChokoLait;

#define CE_OBJECT_COMMON\
	CE_CLASS_COMMON\

#define CE_COMPONENT_COMMON\
	CE_OBJECT_COMMON

#define CE_OBJECT(nm)\
	class _ ## nm;\
	typedef Ref<_ ## nm> nm;\
	typedef std::weak_ptr<_ ## nm> p ## nm;