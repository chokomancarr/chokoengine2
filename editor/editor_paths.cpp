#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::unordered_map<std::string, std::string> EPaths::paths;

void EPaths::Init() {
    const auto& p = IO::userPath() + ".ce2/config/paths.json";
    if (!IO::FileExists(p)) return;
    const auto& data = JsonParser::Parse(IO::ReadFile(p));
    for (auto& d : data.group[0].value.group) {
        paths.emplace(d.key.string, d.value.string);
    }
}

const std::string& EPaths::Get(const std::string& sig, const std::string& def) {
    if (!paths.count(sig)) {
        return def;
    }
    else {
        return paths[sig];
    }
}

CE_END_ED_NAMESPACE