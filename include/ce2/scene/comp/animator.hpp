#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Animator : public _Component { CE_COMPONENT_COMMON
    AnimGraph _graph;

    _AnimGraph::State _state;

	bool _single;

	AnimClip _clip;

	float _clipTime;

	bool _repeat;

public:
	_Animator();

	_Animator(const _Animator& rhs) = default;

	/* The animation graph
	 * Only works when single is false
	*/
    CE_GET_SET_MEMBER_F(graph);

	/* True -> only play the assigned clip
	 * False -> evaluate the assigned graph
	 */
	CE_GET_SET_MEMBER(single);

	/* The animation clip to play
	 * Only works when single is true
	 */
	CE_GET_SET_MEMBER_F(clip);

	/* Is the animation clip repeated
	 * Only works when single is true
	 */
	CE_GET_SET_MEMBER(repeat);

	/* Set variable values of the active graph
	 */
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);

	/* Overrides the current playing state
	 */
	void PlayState(const std::string& name, float time = 0);

	/* Get an entry from the current time frame
	 */
	_AnimClip::VQ Get(const std::string& sig) const;

	void OnLateUpdate() override;
};

CE_END_NAMESPACE