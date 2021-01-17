#include <string>
#include <sstream>
#include <utility>
#include "state/block.hpp"
#include "state/tree.hpp"
#include "state/state_renderer.hpp"

namespace {
    bool isRoot(const Block& block) {
        return block.getParentHash() == Block::EMPTY_HASH;
    }
}

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

std::string StateRenderer::nodeId(const Block& block) const {
    if (currentTreeId.empty()) return "\"" + contentLabeller(block) + "\"";
    return "\"" + currentTreeId + "-" + contentLabeller(block) + "\"";
}

void StateRenderer::renderNodes() const {
    for (const auto&[_, val] : currentHBMapping) {
        auto nodeLabel = contentLabeller(val), id = nodeId(val);
        auto style = (val.getStatus() == Status::PRESENT) ? PRESENT_NODE_STYLE :
                     (val.getStatus() == Status::NOTARIZED) ? NOTARIZED_NODE_STYLE : FINALIZED_NODE_STYLE;
        description << "\t" << id << " [" << style;
        if (isRoot(val)) description << " , label=" << ROOT_SYMBOL;
        else description << " , label=\"" << nodeLabel <<"\"";
        description << "]\n";
    }
}

void StateRenderer::renderEdges() const {
    for (const auto&[_, val] : currentHBMapping) {
        if (isRoot(val)) continue;
        auto currentId = nodeId(val);
        auto parentId = nodeId(currentHBMapping.at(val.getParentHash()));
        description << "\t" << currentId << "->" << parentId << "\n";
    }
}

void StateRenderer::setCurrentTreeProperties(const StateRenderer::named_tree_t& namedTree) const {
    currentHBMapping = namedTree.first.hbMapping;
    currentTreeId = namedTree.second;
}
