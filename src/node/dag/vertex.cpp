#include "node/dag/vertex.hpp"

Vertex::Vertex(const Hashable& content) :
        content(content),
        parent(*this) {}

Vertex::Vertex(const Hashable& content, const Vertex& parent) :
        content(content),
        parent(parent) {}

const Hashable& Vertex::getContent() const { return content; }

const Vertex& Vertex::getParent() const { return parent; }

Status Vertex::getStatus() const { return status; }

void Vertex::notarize() const { status = Status::NOTARIZED; }

void Vertex::finalize() const { status = Status::FINALIZED; }
