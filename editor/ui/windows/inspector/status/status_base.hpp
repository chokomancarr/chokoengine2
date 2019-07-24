#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_I_Status;
typedef std::shared_ptr<EW_I_Status> pEW_I_Status;

class EW_I_Status {
public:
    class UMap {
    public:
        std::unordered_map<ChokoEngine::objectid, pEW_I_Status> _umap = {};

        void Add(ChokoEngine::objectid, const pEW_I_Status&);
        void Erase(ChokoEngine::objectid);
        template <typename T>
        std::shared_ptr<T> Get(ChokoEngine::objectid);
    };
};

CE_END_ED_NAMESPACE

#include "status_base.inl"