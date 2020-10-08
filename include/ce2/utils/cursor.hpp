#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Cursor {
	CE_CLASS_COMMON

	static CE_EXPORT bool _locked;
	static CE_EXPORT bool _visible;

public:
	
	//is the cursor position updated
	//if this is false, Input::mousePosition
	//will remain the same as at time of setting
	CE_GET_SET_ST_MEMBER(locked);

	//is the cursor displayed
	CE_GET_SET_ST_MEMBER_F(visible);

};

CE_END_NAMESPACE