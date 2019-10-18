#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Object : public std::enable_shared_from_this<_Object> {
    static ChokoEngine::objectid _idCount;

protected:
    _Object(const std::string& nm = "Unnamed Object");

	bool _loading = false;
    bool _deleted = false;

    std::string _name;
    const ChokoEngine::objectid _id;
public:
    virtual ~_Object() = default;

    CE_GET_SET_MEMBER(name);
    CE_GET_MEMBER(id);

	/* If an object is currently loading,
	 * issues a blocking call until it finishes
	 */
	virtual void Wait() {};

    template <class T>
    friend class Ref;
	template <class T>
	friend class Ref_w;
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