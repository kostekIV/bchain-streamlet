#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "state/tree/tree.hpp"


class TreeUnion {
public:
    TreeUnion();

    TreeUnion& add(const Tree& tree);

    bool isValidTree() const;

    bool isConnected() const;

    bool isFinalizedCorrectly() const;

    bool operator==(TreeUnion const& rhs) const;
    bool operator!=(TreeUnion const& rhs) const;

    /**
     * Expect valid mapping only if isValidTree() returns true
     */
    std::unordered_map<hash_t, const Vertex> asHashVertexMapping() const;

private:
    void insertTree(const Tree &tree);
    void safeInsertEdge(const hash_t& v, const hash_t& u);
    void safeInitialiseEdges(const hash_t& v);

    unsigned rootEpoch;
    hash_t root;
    std::unordered_set<hash_t> nodes;
    std::unordered_map<hash_t, std::unordered_set<hash_t>> edges;
    std::unordered_map<hash_t, unsigned> nodesEpoch;
    std::unordered_map<hash_t, Status> nodesStatus;
};
