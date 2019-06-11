#pragma once
#include "chokoengine.hpp"

#define REFCNT_DESTRUCTOR(nm)\
	~nm() { CheckUniqueRef(); }\
protected:\
	void DestroyRef() override;\
public:

BEGIN_CE_NAMESPACE

class RefCnt {
	std::shared_ptr<char> _ptr;

protected:
	RefCnt();
	RefCnt(RefCnt const& rhs);

	RefCnt& operator= (RefCnt const& rhs);

	void CheckUniqueRef();

	virtual void DestroyRef() = 0;
};

END_CE_NAMESPACE