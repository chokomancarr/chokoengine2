Adding a component
==================================

List of files to modify
-----------------------

include/ce2/

 - chokoengine_includes.hpp
 - scene/component.hpp
 - scene/enums/component_types.hpp

src/

 - asset/prefab/prefab_comp.[hpp, cpp]

editor/

 - ui/windows/inspector/component.[hpp, cpp]
 
List of files to add
--------------------

- include/ce2/scene/comp/[type].hpp
- src/asset/prefab/comp/[type].cpp
- src/scene/comp/[type].cpp
- editor/ui/windows/inspector/comp/[type].cpp
- editor/ui/windows/inspector/status/comp/[type].hpp