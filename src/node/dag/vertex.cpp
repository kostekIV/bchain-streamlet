#include <node/dag/vertex.hpp>

Vertex::Vertex(const Hashable& content, const Vertex& parent) :
        content(content),
        parent(parent) {}

const Hashable& Vertex::getContent() const { return content; }

const Vertex& Vertex::getParent() const { return parent; }

Status Vertex::getStatus() const { return status; }

void Vertex::setStatus(Status status) const { this->status = status; }
