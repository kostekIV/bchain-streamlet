#pragma once

#include <sstream>

#include "tree-test.hpp"

class StateRenderer {
    using mapping_t = std::function<std::string(const Hashable&)>;
    using named_tree_t = std::pair<const Tree&, std::string>;

public:
    explicit StateRenderer(mapping_t contentLabeller);

    std::string renderTree(const named_tree_t& namedTree) const;

    std::string renderForest(const std::vector<named_tree_t>& namedTrees) const;

private:
    std::string renderWithTitle(const std::string& title) const;

    void renderNodes() const;

    void renderEdges() const;

    std::string nodeId(const Vertex& v) const;

    void setCurrentTreeProperties(const named_tree_t& namedTree) const;

    const mapping_t contentLabeller;

    mutable std::ostringstream description;
    mutable std::unordered_map<hash_t, const Vertex> currentHVMapping;
    mutable std::string currentTreeId;

    const static std::string PRESENT_NODE_STYLE, NOTARIZED_NODE_STYLE, FINALIZED_NODE_STYLE, ROOT_SYMBOL;
};