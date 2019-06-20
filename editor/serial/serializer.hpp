#pragma once
#include "chokoeditor.hpp"
#include "serialized_entry.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializer {
public:
    static std::vector<ESerializedGroup> entryGroups;

	static void Init();

    static void AddEntry(const std::string& name, size_t id, pESerializedEntry entry);
};

CE_END_ED_NAMESPACE

#include "config/assets.hpp"
#include "config/components.hpp"