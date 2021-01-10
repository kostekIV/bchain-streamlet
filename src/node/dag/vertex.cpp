#include <cassert>
#include "node/dag/vertex.hpp"

Vertex::Vertex(const Hashable& content) :
        content(content),
        parent(*this),
        depth(0) {}

Vertex::Vertex(const Hashable& content, const Vertex& parent) :
        content(content),
        parent(parent),
        depth(parent.depth + 1) {}

const Hashable& Vertex::getContent() const { return content; }

const Vertex& Vertex::getParent() const { return parent; }

unsigned Vertex::getDepth() const { return depth; }

Status Vertex::getStatus() const { return status; }

void Vertex::notarize() const {
    assert(status == Status::PRESENT);
    status = Status::NOTARIZED;
}

void Vertex::finalize() const {
    assert(status == Status::NOTARIZED or status == Status::FINALIZED);
    status = Status::FINALIZED;
}
