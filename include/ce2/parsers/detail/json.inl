#pragma once
#include "chokoengine.hpp"
#include "parsers/json.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
T JsonObject::ToEnum(const std::initializer_list<std::string>& entries) {
    int i = 0;
    for (auto& e : entries) {
        if (e == string)
            return (T)i;
    }
	Debug::Warning("JsonObject", "Value \"" + string + "\" does not match any enum values!");
    std::string ents = "";
    for (auto& e : entries) {
        ents += " " + e + ",";
    }
    ents.pop_back();
    Debug::Warning("JsonObject", "Allowed values are:" + ents);
    return (T)0;
}

CE_END_NAMESPACE