#include "Node.h"
#include "Context.h"

AST::Node::Node() : nodeType(AST::NodeType::NOTYPE) {};

AST::Node::Node(AST::NodeType type) : nodeType(type) {};

AST::NodeType AST::Node::getNodeType() const
{
	return nodeType;
}

const void AST::Node::addChild(std::unique_ptr<AST::Node> child)
{
	children.push_back(std::move(child));
}

AST::Node* AST::Node::getChild(const int& index) const
{
	return children[index].get();
}

const std::size_t AST::Node::getChildrenSize() const
{
    return children.size();
}

void AST::Node::releaseChild(const int &index)
{
    children[index].release();
}
