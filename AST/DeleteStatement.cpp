#include "DeleteStatement.h"
#include "Node.h"

#include "Context.h"

AST::DeleteStatement::DeleteStatement(std::string id) : AST::Node(AST::NodeType::DeleteStatement), identifier(id)
{
}

void AST::DeleteStatement::execute(Context *context)
{
    context->removeVariable(identifier);
}
