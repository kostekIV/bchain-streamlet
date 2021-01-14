#include <string>
#include <sstream>
#include <utility>
#include "state/tree/state_renderer.hpp"
#include "state/tree/tree.hpp"

const std::string StateRenderer::PRESENT_NODE_STYLE("style=\"rounded,bold\"");
const std::string StateRenderer::NOTARIZED_NODE_STYLE("style=\"rounded,bold\", peripheries=2");
const std::string StateRenderer::FINALIZED_NODE_STYLE(
        "style=\"rounded,bold,filled\", fillcolor=antiquewhite1, peripheries=2");

const std::string StateRenderer::ROOT_SYMBOL("<&perp;>");

StateRenderer::StateRenderer(StateRenderer::mapping_t contentLabeller) : contentLabeller(std::move(contentLabeller)) {}

std::string StateRenderer::renderTree(const StateRenderer::named_tree_t& namedTree) const {
    description = std::ostringstream();
    setCurrentTreeProperties(namedTree);
    return renderWithTitle("digraph Tree " + currentTreeId);
}

std::string StateRenderer::renderForest(const std::vector<named_tree_t>& namedTrees) const {
    description = std::ostringstream();
    for (const auto& namedTree : namedTrees) {
        setCurrentTreeProperties(namedTree);
        renderWithTitle("subgraph cluster_" + currentTreeId);
    }
    return "digraph Forest {\n" + description.str() + "}\n";
}

std::string StateRenderer::renderWithTitle(const std::string& title) const {
    description << title << " {\n";
    description << "label=\"" << currentTreeId << "\"\n";
    description << "node [shape=box]\n";
    renderNodes();
    renderEdges();
    description << "}\n";
    return description.str();
}

std::string StateRenderer::nodeId(const Vertex& v) const {
    if (currentTreeId.empty()) return "\"" + contentLabeller(v.getContent()) + "\"";
    return "\"" + currentTreeId + "-" + contentLabeller(v.getContent()) + "\"";
}

void StateRenderer::renderNodes() const {
    for (const auto&[_, val] : currentHVMapping) {
        auto nodeLabel = contentLabeller(val.getContent()), id = nodeId(val);
        auto style = (val.getStatus() == Status::PRESENT) ? PRESENT_NODE_STYLE :
                     (val.getStatus() == Status::NOTARIZED) ? NOTARIZED_NODE_STYLE : FINALIZED_NODE_STYLE;
        description << "\t" << id << " [" << style;
        if (Tree::isRoot(val)) description << " , label=" << ROOT_SYMBOL;
        else description << " , label=" << nodeLabel;
        description << "]\n";
    }
}

void StateRenderer::renderEdges() const {
    for (const auto&[_, val] : currentHVMapping) {
        if (Tree::isRoot(val)) continue;
        auto currentId = nodeId(val);
        auto parentId = nodeId(val.getParent());
        description << "\t" << currentId << "->" << parentId << "\n";
    }
}

void StateRenderer::setCurrentTreeProperties(const StateRenderer::named_tree_t& namedTree) const {
    currentHVMapping = namedTree.first.hvMapping;
    currentTreeId = namedTree.second;
}
