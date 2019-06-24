#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

Texture EImages::background;
Texture EImages::logo;

void EImages::Init() {
#define IIMG(vr, nm) vr = Texture::New(IO::path() + "res/" nm)
	IIMG(background, "background.jpg");
	IIMG(logo, "logo.png");
}

CE_END_ED_NAMESPACE