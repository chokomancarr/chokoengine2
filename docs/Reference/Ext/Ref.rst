Ref
==================

Definition
----------

.. class:: template<class T> Ref

    Wrapper for instantiatable objects. All objects must be constructed with this wrapper instead of using the raw constructor or smart pointers.

Constructors
------------

.. function:: template<class T> Ref<T> Ref<T>::Ref()
    template<class T> Ref<T> Ref<T>::Ref(std::nullptr_t)

    Creates an empty object
    
.. function:: template<class T, class U, class = std::enable_if<std::is_base_of<T, U>::value, U>::type*> Ref<T> Ref<T>::Ref(const Ref<U>& rhs)
    template<class T, class U, class = std::enable_if<!std::is_base_of<T, U>::value, U>::type*> explicit Ref<T> Ref<T>::Ref(const Ref<U>& rhs)
    
    Assigns the object from another reference. If :expr:`rhs` is of a child class type, conversion may be implicit.
    
Variable functions
------------------

Method functions
----------------

.. function:: template<class T, class... Args> Ref<T> Ref<T>::New(Args&&... args)

    Constructs a new object from the specified arguments. Refer to the actual object's documentation for the argument lists.
 
.. function:: template<class T> Ref<T>& Ref<T>::operator =(const Ref<T>& rhs)

    Assignment operator

.. function:: template<class T> T* Ref<T>::operator ->() const

    Dereferences the object
    
.. function:: template<class T> bool Ref<T>::operator !() const

    Checks if there is a valid object in this reference. Note that you should call this instead of :expr:`!object()` as live objects may be marked for deletion.
    
.. function:: template<class T> bool Ref<T>::operator ==(const Ref<T>& rhs) const
    template<class T> bool Ref<T>::operator !=(const Ref<T>& rhs) const
    
    Checks if the two objects contain the same reference, or are both empty.
