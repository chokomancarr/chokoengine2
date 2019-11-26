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

	virtual void _CopyInfo(const Object& tar);

	template <typename T>
	Ref<T> get_shared() const;
public:
    virtual ~_Object() = default;

    CE_GET_SET_MEMBER(name);
    CE_GET_MEMBER(id);

	/* If an object is currently loading,
	 * issues a blocking call until it finishes
	 */
	virtual void Wait() {};

	/* Explicitly deletes an object
	 * After deletion, all references will return nullptrs
	 */
	virtual void Delete();

    template <class T>
    friend class Ref;
	template <class T>
	friend class Ref_w;
};

CE_END_NAMESPACE

#include "detail/object_base.inl"