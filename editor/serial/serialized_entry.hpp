#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

enum class ESerializedEntryType {
    Float,
    Int,
    Vec2,
    Vec3,
    ObjRef,
    CompRef
};

class ESerializedEntry {
public:
    virtual ~ESerializedEntry() = default;

    virtual ESerializedEntryType type() const = 0;

    virtual std::string Get(pObject o) const = 0;
    virtual void Set(pObject o, const std::string&) = 0;
};

typedef std::shared_ptr<ESerializedEntry> pESerializedEntry;

class ESerializedGroup {
public:
    ESerializedGroup(const std::string& nm, size_t id)
            : classSignature(nm), classId(id) {}

    std::string classSignature;
    size_t classId;

    std::vector<pESerializedEntry> entries;
};

template <typename C, typename T>
class _ESerializedEntry : public ESerializedEntry {
public:
    virtual ~_ESerializedEntry() = default;

    T (C::*getter)(void) const;
    void (C::*setter)(const T&);

    C* get_ptr(const pObject& o) const {
        return std::dynamic_pointer_cast<C>(o).get();
    }
};

template <typename C>
class ESerializedEntry_F : public _ESerializedEntry<C, float> {
public:
    ESerializedEntryType type() const override {
        return ESerializedEntryType::Float;
    }

    std::string Get(pObject o) const override {
        return std::to_string((this->get_ptr(o)->*this->getter)());
    }
    void Set(pObject o, const std::string& s) override {
        (this->get_ptr(o)->*this->setter)(std::stof(s));
    }
};

CE_END_ED_NAMESPACE