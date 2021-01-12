#include <cassert>
#include "logging/easylogging++.h"

#include "state/tree/vertex.hpp"

Vertex::Vertex(const Hashable& content) :
        content(content),
        depth(0),
        parent(*this) {}

Vertex::Vertex(const Hashable& content, const Vertex& parent) :
        content(content),
        depth(parent.depth + 1),
        parent(parent) {}

const Hashable& Vertex::getContent() const { return content; }

const Vertex& Vertex::getParent() const { return parent; }

unsigned Vertex::getDepth() const { return depth; }

Status Vertex::getStatus() const { return status; }

void Vertex::notarize() const {
    LOG(DEBUG) << "[Vertex]: notarizing vertex with status " << statusToString(status);
    assert(status == Status::PRESENT);
    status = Status::NOTARIZED;
}

void Vertex::finalize() const {
    LOG(DEBUG) << "[Vertex]: finalizing vertex with status " << statusToString(status);
    assert(status == Status::NOTARIZED or status == Status::FINALIZED);
    status = Status::FINALIZED;
}
