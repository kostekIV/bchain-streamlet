#pragma once

#include <sstream>

#include "tree.hpp"

class TreeRenderer {
    using mapping_t = std::function<std::string(const Hashable&)>;
public:
    TreeRenderer(const Tree& tree, mapping_t contentLabeller);

    std::string render();

private:
    void renderNodes();

    void renderEdges();

    mapping_t contentLabeller;
    const Tree& tree;
    std::ostringstream description;

    static std::string PRESENT_NODE_STYLE, NOTARIZED_NODE_STYLE, FINALIZED_NODE_STYLE, ROOT_SYMBOL;
};
