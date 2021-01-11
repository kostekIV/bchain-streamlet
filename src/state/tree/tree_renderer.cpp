#include <string>
#include <sstream>
#include <utility>
#include "state/tree/tree_renderer.hpp"
#include "state/tree/tree.hpp"


TreeRenderer::TreeRenderer(const Tree& tree, mapping_t contentLabeller) :
        tree(tree),
        contentLabeller(std::move(contentLabeller)) {}

std::string TreeRenderer::render() {
    description = std::ostringstream{};
    description << "digraph G{\n";
    description << "node [shape=box]\n";
    renderNodes();
    renderEdges();
    description << "}\n";
    return description.str();
}

void TreeRenderer::renderNodes() {
    for (const auto&[key, val] : tree.hvMapping) {
        auto nodeLabel = contentLabeller(val.getContent());
        auto style = (val.getStatus() == Status::PRESENT) ? "style=\"rounded,bold\"" :
                     (val.getStatus() == Status::NOTARIZED) ? "style=\"rounded,bold\", peripheries=2" :
                     "style=\"rounded,bold,filled\", fillcolor=antiquewhite1, peripheries=2";
        description << "\t" << nodeLabel << " [" << style;

        if (Tree::isRoot(val)) description << " , label=<&perp;>";
        description << "]\n";
    }
}

void TreeRenderer::renderEdges() {
    for (const auto&[key, val] : tree.hvMapping) {
        if (Tree::isRoot(val)) continue;
        auto currentLabel = contentLabeller(val.getContent());
        auto parentLabel = contentLabeller(val.getParent().getContent());
        description << "\t" << currentLabel << "->" << parentLabel << "\n";
    }
}

