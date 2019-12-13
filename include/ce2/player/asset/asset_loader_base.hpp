#pragma once
#include "chokoplayer.hpp"

/*
 * The program must implement the asset loader derived from this class,
 * and set it to _AssetLoaderBase::instance.
 */

CE_BEGIN_PL_NAMESPACE

class _AssetLoaderBase {
public:
    static CE_EXPORT AssetLoaderBase instance;

    virtual Asset Load(AssetType, const std::string&) = 0;

    virtual ~_AssetLoaderBase() = default;
};

CE_END_PL_NAMESPACE