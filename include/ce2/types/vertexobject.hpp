#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _VertexObject {
    GLuint _pointer;
    std::vector<VertexBuffer> _buffers;
    int _bufferSize;
    
    _VertexObject(const _VertexObject&) = delete;
public:
    _VertexObject();
	~_VertexObject();

    CE_GET_MEMBER(pointer);
    const VertexBuffer& buffer(int i) const;
    CE_GET_MEMBER(bufferSize);

    void AddBuffer(const VertexBuffer& buf);
    void Bind() const;
    void Unbind() const;
};

typedef std::shared_ptr<_VertexObject> VertexObject;
VertexObject VertexObject_New();

CE_END_NAMESPACE