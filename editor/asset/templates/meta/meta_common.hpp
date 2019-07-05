#pragma once
#include "chokoeditor.hpp"

#define CE_BEGIN_META_NAMESPACE CE_BEGIN_ED_NAMESPACE namespace meta {

#define CE_END_META_NAMESPACE } CE_END_ED_NAMESPACE

CE_BEGIN_META_NAMESPACE

const std::string header = R"({
  "signature":"ChokoEngine Asset MetaData",
  "version":"20",
)";

CE_END_META_NAMESPACE

#include "material.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "prefab.hpp"