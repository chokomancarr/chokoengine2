#include "chokoengine.hpp"
#include "enums/component_types.hpp"
#include "enums/callback_flags.hpp"

CE_BEGIN_NAMESPACE

class _Component : public _Object { CE_OBJECT_COMMON
public:

protected:
	_Component(ComponentType t, const std::string& nm);

	pSceneObject _object;

	CompCallbackFlag::Tp _callbackMask;

	virtual Component Clone() const = 0;
public:
	virtual ~_Component() = default;

	const ComponentType componentType;

	SceneObject object();

	/* Called during the first frame after this object
	 * is spawned, before the Update loop.
	 * Note: OnUpdate will also be called on the same frame.
	 */
	virtual void OnStart() {}

	/* Called during the main logic loop
 	*/
	virtual void OnUpdate() {}

	/* Called during the main logic loop (2nd pass)
	 */
	virtual void OnLateUpdate() {}

	/* Called during the physics loop
	*/
	virtual void OnPhysicsUpdate() {}

	/* Called after the main logic loop, and other loops (last pass)
	 */
	virtual void OnPostLogic() {}

	/* Called right before rendering objects into G-buffers
	 * This function will only be called if the component
	 * is attached to the camera
	 */
	virtual void OnPreRender() {}

	/* Called after rendering G-buffers, before renering
	 * into the blit buffer
	 * This function will only be called if the component
	 * is attached to the camera
	 */
	virtual void OnPreBlit() {}

	/* Called after rendering all effects to the blit buffer,
	 * before copying to the target
	 * This function will only be called if the component
	 * is attached to the camera
	 * Rendering in this function will target the blit buffer
	 */
	virtual void OnPostBlit() {}

	/* Called after all rendering processes are completed
	 * This function will only be called if the component
	 * is attached to the camera
	 */
	virtual void OnPostRender() {}

	/* Called when the main window is redrawn
	 * UI functions to draw onto the screen
	 * should be called in this function
	 */
	virtual void OnPaint() {}

	friend class _Scene;
	friend class _SceneObject;
	friend class _PrefabComp;
};

CE_END_NAMESPACE

#define CE_COMPDEF(nm) _Component(ComponentType::nm, #nm)

#include "comp/animator.hpp"
#include "comp/camera.hpp"
#include "comp/light.hpp"
#include "comp/light_probe.hpp"
#include "comp/mesh_renderer.hpp"
#include "comp/particle_system.hpp"
#include "comp/rig.hpp"
#include "comp/script.hpp"