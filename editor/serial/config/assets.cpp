#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void Export() {
    auto e = std::make_shared<ESerializedEntry_F<_Camera>>();
    e->getter = static_cast<decltype(e->getter)>(&_Camera::fov);
    auto oid = ((_Camera*)0)->_object_id();
    ESerializer::AddEntry("Camera", oid, e);
}

CE_END_ED_NAMESPACE