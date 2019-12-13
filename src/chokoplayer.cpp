#include "chokoplayer_inc.hpp"

CE_BEGIN_PL_NAMESPACE

inline void paint() {

}

void ChokoPlayer::Init() {
	
}

void ChokoPlayer::Main() {

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_PL_NAMESPACE
