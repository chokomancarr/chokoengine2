#include "serializer.hpp"

CE_BEGIN_ED_NAMESPACE

std::vector<ESerializedGroup> ESerializer::entryGroups;

void ESerializer::Init() {
	ES_Assets::Register();
	ES_Components::Register();
}

void ESerializer::AddEntry(const std::string& name, size_t id, pESerializedEntry entry) {
    auto g = std::find_if(entryGroups.begin(), entryGroups.end(), [&](const ESerializedGroup& g) {
        return g.classId == id;
    });
    if (g == entryGroups.end()) {
        entryGroups.push_back(ESerializedGroup(name, id));
        g = entryGroups.end() - 1;
    }

}

CE_END_ED_NAMESPACE