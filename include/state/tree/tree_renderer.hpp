#pragma once

#include <sstream>

#include "tree.hpp"

class TreeRenderer {
    using mapping_t = std::function<std::string(const Hashable&)>;
    using named_tree_t = std::pair<const Tree&, std::string>;
public:
    static std::string renderForest(const std::vector<named_tree_t>& trees, const mapping_t& contentLabeller);

    TreeRenderer(const Tree& tree, mapping_t contentLabeller, std::string treeId = "");

    std::string render() const;

private:
    std::string renderWithinForest() const;

    std::string renderWithTitle(const std::string& title) const;

    void renderNodes() const;

    void renderEdges() const;

    std::string nodeId(const Vertex& v) const;

    const mapping_t contentLabeller;
    const Tree& tree;
    const std::string treeId;
    mutable std::ostringstream description;

    const static std::string PRESENT_NODE_STYLE, NOTARIZED_NODE_STYLE, FINALIZED_NODE_STYLE, ROOT_SYMBOL;
};
