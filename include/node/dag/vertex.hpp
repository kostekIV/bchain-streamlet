#pragma once

#include <node/hashable.hpp>
#include "status.hpp"

class Vertex {
public:
    Vertex(const Hashable& content, const Vertex& parent);

    const Hashable& getContent() const;

    const Vertex& getParent() const;

    Status getStatus() const;
    void setStatus(Status status) const;

private:
    const Hashable& content;
    const Vertex& parent;
    mutable Status status;
};
