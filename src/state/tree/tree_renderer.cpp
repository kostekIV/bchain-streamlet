#include <string>
#include <sstream>
#include <utility>
#include "state/tree/tree_renderer.hpp"
#include "state/tree/tree.hpp"

const std::string TreeRenderer::PRESENT_NODE_STYLE("style=\"rounded,bold\"");
const std::string TreeRenderer::NOTARIZED_NODE_STYLE("style=\"rounded,bold\", peripheries=2");
const std::string TreeRenderer::FINALIZED_NODE_STYLE(
        "style=\"rounded,bold,filled\", fillcolor=antiquewhite1, peripheries=2");

const std::string TreeRenderer::ROOT_SYMBOL("<&perp;>");

TreeRenderer::TreeRenderer(const Tree& tree, mapping_t contentLabeller, std::string treeId) :
        tree(tree),
        treeId(std::move(treeId)),
        contentLabeller(std::move(contentLabeller)) {}

std::string TreeRenderer::render() const { return renderWithTitle("digraph Tree " + treeId); }

std::string TreeRenderer::renderWithinForest() const { return renderWithTitle("subgraph cluster_" + treeId); }

std::string TreeRenderer::renderWithTitle(const std::string& title) const {
    description = std::ostringstream{};
    description << title << " {\n";
    description << "label=\"" << treeId << "\"\n";
    description << "node [shape=box]\n";
    renderNodes();
    renderEdges();
    description << "}\n";
    return description.str();
}

std::string TreeRenderer::nodeId(const Vertex& v) const {
    if (treeId.empty()) return "\"" + contentLabeller(v.getContent()) + "\"";
    return "\"" + treeId + "-" + contentLabeller(v.getContent()) + "\"";
}

void TreeRenderer::renderNodes() const {
    for (const auto&[key, val] : tree.hvMapping) {
        auto nodeLabel = contentLabeller(val.getContent()), id = nodeId(val);
        auto style = (val.getStatus() == Status::PRESENT) ? PRESENT_NODE_STYLE :
                     (val.getStatus() == Status::NOTARIZED) ? NOTARIZED_NODE_STYLE : FINALIZED_NODE_STYLE;
        description << "\t" << id << " [" << style;
        if (Tree::isRoot(val)) description << " , label=" << ROOT_SYMBOL;
        else description << " , label=" << nodeLabel;
        description << "]\n";
    }
}

void TreeRenderer::renderEdges() const {
    for (const auto&[key, val] : tree.hvMapping) {
        if (Tree::isRoot(val)) continue;
        auto currentId = nodeId(val);
        auto parentId = nodeId(val.getParent());
        description << "\t" << currentId << "->" << parentId << "\n";
    }
}

std::string TreeRenderer::renderForest(const std::vector<named_tree_t>& trees, const mapping_t& contentLabeller) {
    auto description = std::ostringstream{};
    description << "digraph Forest {\n";
    for (const auto&[tree, treeId] : trees)
        description << TreeRenderer(tree, contentLabeller, treeId).renderWithinForest();
    description << "}\n";
    return description.str();
}
