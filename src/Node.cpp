#include "Node.h"

Node::Node() : nodeType(NodeType::NOTYPE) {};

Node::Node(NodeType type) : nodeType(type) {};

NodeType Node::getNodeType() const
{
	return nodeType;
}

const void Node::addChild(std::unique_ptr<Node> child)
{
	children.push_back(std::move(child));
}

Node* Node::getChild(const int& index) const
{
	return children[index].get();
}

const std::size_t Node::getChildrenSize() const
{
	return children.size();
}
