#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Object : public std::enable_shared_from_this<Object> {
protected:
    Object(const std::string& nm = "Unnamed Object");

    std::string _name;
public:
    CE_GET_SET_MEMBER(name);
};

CE_END_NAMESPACE