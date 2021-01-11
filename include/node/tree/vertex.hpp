#pragma once

#include "node/hashable.hpp"
#include "status.hpp"

class Vertex {
public:
    explicit Vertex(const Hashable& content);
    Vertex(const Hashable& content, const Vertex& parent);

    const Hashable& getContent() const;

    const Vertex& getParent() const;

    unsigned getDepth() const;

    Status getStatus() const;
    void notarize() const;
    void finalize() const;

private:
    const Hashable& content;
    const Vertex& parent;
    const unsigned depth;
    mutable Status status = Status::PRESENT;
};