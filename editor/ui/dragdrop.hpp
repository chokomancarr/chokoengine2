#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EDragDrop {
public:
    enum class Type {
        Asset,
        SceneObject
    };
    static Type type;
    static EAssetList::TypeOfSt assetType;

    static std::vector<std::string> target;

    static void Set(Type, const std::vector<std::string>&);
    static void Set(EAssetList::TypeOfSt, const std::vector<std::string>&);
    static bool IsEmpty();
    static bool IsSingle();
    static void Clear();

    static void PostLoop();
};

CE_END_ED_NAMESPACE
