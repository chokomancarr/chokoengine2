#pragma once

#define CE_OBJECT(nm)\
	class _ ## nm;\
	typedef Ref<_ ## nm> nm;\
	typedef std::weak_ptr<_ ## nm> p ## nm;