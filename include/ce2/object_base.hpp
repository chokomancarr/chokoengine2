#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Object : public std::enable_shared_from_this<Object> {
protected:
    Object(const std::string& nm = "Unnamed Object");

    std::string _name;
public:
    virtual ~Object() = default;

    virtual size_t _object_id() const { return __COUNTER__; }

    CE_GET_SET_MEMBER(name);
};

CE_END_NAMESPACE