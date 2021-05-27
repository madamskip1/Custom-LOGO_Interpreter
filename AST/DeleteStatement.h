#ifndef DELETESTATEMENT_H
#define DELETESTATEMENT_H

#include <string>
#include "Node.h"

namespace AST
{
    class DeleteStatement
            : public Node
    {
    public:
        DeleteStatement() = delete;
        DeleteStatement(std::string id);

        virtual void execute(Context* context);

    private:
        std::string identifier;
    };
}

#endif // DELETESTATEMENT_H
