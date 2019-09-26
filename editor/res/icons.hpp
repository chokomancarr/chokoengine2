#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EIcons {
public:
	static std::unordered_map<std::string, Texture> icons;

	static void Init();
};

CE_END_ED_NAMESPACE