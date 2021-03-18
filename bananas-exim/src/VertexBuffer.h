#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H
#endif  VERTEX_BUFFER_H


class VertexBuffer {

    unsigned int m_RenderId;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

};