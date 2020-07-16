#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class JsonUtils {
public:
	/* Returns the subset of a that is different from b
	 */
	JsonObject Diff(const JsonObject& a, const JsonObject& b);
};

CE_END_ED_NAMESPACE