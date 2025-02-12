#pragma once
#include "chokoengine.hpp"
#include "enums/asset_types.hpp"

CE_BEGIN_NAMESPACE

class _Asset : public _Object { CE_OBJECT_COMMON
protected:
    _Asset(AssetType);

    std::string _assetSignature;
public:
    virtual ~_Asset() = default;

	const AssetType assetType;

	bool readonly;

    CE_GET_SET_MEMBER(assetSignature);
};

CE_END_NAMESPACE

#include "./animclip.hpp"
#include "./animgraph.hpp"
#include "./armature.hpp"
#include "./font.hpp"
#include "./mesh.hpp"
#include "./prefab.hpp"
#include "./shader.hpp"
#include "./material.hpp"
#include "./prefab.hpp"
#include "./texture.hpp"
#include "./texture3.hpp"
#include "./background.hpp"
#include "./cubemap.hpp"
#include "./render_target.hpp"
#include "./render_target3.hpp"