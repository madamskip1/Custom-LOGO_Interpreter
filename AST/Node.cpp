#include "Node.h"
#include "../include/Context.h"
#include "Token.h"

AST::Node::Node() : nodeType(AST::NodeType::NOTYPE) {};

AST::Node::Node(AST::NodeType type) : nodeType(type) {};

AST::NodeType AST::Node::getNodeType() const
{
	return nodeType;
}

void AST::Node::addChild(std::unique_ptr<AST::Node> child)
{
	children.push_back(std::move(child));
}

AST::Node* AST::Node::getChild(const int& index) const
{
	return children[index].get();
}

std::size_t AST::Node::getChildrenSize() const
{
	return children.size();
}

void AST::Node::releaseChild(const int& index)
{
    children[index].release();
}

void AST::Node::setToken(Token token)
{
    this->token = token;
}

