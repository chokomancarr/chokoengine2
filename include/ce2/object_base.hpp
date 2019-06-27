#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Object : public std::enable_shared_from_this<Object> {
protected:
    Object(const std::string& nm = "Unnamed Object");

    bool _deleted = false;

    std::string _name;
public:
    virtual ~Object() = default;

    CE_GET_SET_MEMBER(name);

    template <class T>
    friend class Ref;
    template <class T>
    friend void DeleteObject(Ref<T>& obj);
};

/* Explicitly deletes an object
 * After deletion, all references will return nullptrs
 */
template <class T>
void DeleteObject(Ref<T>& obj);

CE_END_NAMESPACE

#include "detail/object_base.inl"