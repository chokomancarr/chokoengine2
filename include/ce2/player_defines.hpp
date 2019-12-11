#pragma once

#define CE_BEGIN_PL_NAMESPACE CE_BEGIN_NAMESPACE\
	namespace Player {

#define CE_END_PL_NAMESPACE\
	} CE_END_NAMESPACE

#define CE_PL_NS ::CE_NAMESPACE::Player::
	
#define CE_PL_OBJ(nm) class _ ## nm;\
	typedef std::shared_ptr<_ ## nm> nm;
