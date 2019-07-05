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

    virtual std::string Get(Object::_TpPtr o) const = 0;
    virtual void Set(Object::_TpPtr o, const std::string&) = 0;
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
    typedef const T& _rct;

    virtual ~_ESerializedEntry() = default;

    std::string funcSignature;

    _rct (C::*getter)(void) const;
    void (C::*setter)(_rct);

    C* get_ptr(const Object::_TpPtr& o) const {
        return std::dynamic_pointer_cast<C>(o).get();
    }
};

template <typename C>
class ESerializedEntry_F : public _ESerializedEntry<C, float> {
public:
    ESerializedEntryType type() const override {
        return ESerializedEntryType::Float;
    }

    std::string Get(Object::_TpPtr o) const override {
        return std::to_string((this->get_ptr(o)->*this->getter)());
    }
    void Set(Object::_TpPtr o, const std::string& s) override {
        (this->get_ptr(o)->*this->setter)(std::stof(s));
    }
};

CE_END_ED_NAMESPACE