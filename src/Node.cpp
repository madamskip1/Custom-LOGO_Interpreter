#include "Node.h"

Node::Node() : nodeType(NodeType::NOTYPE) {};

Node::Node(NodeType type) : nodeType(type) {};

NodeType Node::getNodeType() const
{
	return nodeType;
}