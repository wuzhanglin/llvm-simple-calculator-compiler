#include "SemanticAnalyzer.h"

#include "Lexer.h"

#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"

namespace
{
    class DeclarationChecker : public ASTVisitor
    {
    public:

        DeclarationChecker() :
            hasError(false)
        {
        }

        bool HasError()
        {
            return hasError;
        }

        void Visit(Factor& node) override
        {
            if (node.GetType() == Factor::kIdent)
            {
                if (scope.find(node.GetValue()) == scope.end())
                {
                    AddError(kNotDeclared, node.GetValue());
                }
            }
        };

        void Visit(BinaryOp& node) override
        {
            if (node.GetLeft())
            {
                node.GetLeft()->Accept(*this);
            }
            else
            {
                hasError = true;
            }

            if (node.GetRight())
            {
                node.GetRight()->Accept(*this);
            }
            else
            {
                hasError = true;
            }
        };

        void Visit(WithDeclaration& node) override
        {
            for (auto variable : node)
            {
                if (!scope.insert(variable).second)
                {
                    AddError(kDeclaredTwice, variable);
                }
            }

            if (node.GetExpr())
            {
                node.GetExpr()->Accept(*this);
            }
            else
            {
                hasError = true;
            }
        };

    private:

        enum ErrorType
        {
            kDeclaredTwice,
            kNotDeclared
        };

    private:

        void AddError(ErrorType errorType, llvm::StringRef variable)
        {
            llvm::errs() << "Variable " << variable << " " << (errorType == kDeclaredTwice ? "already" : "not")
                         << " declared\n";
            hasError = true;
        }

    private:

        llvm::StringSet<> scope;
        bool hasError;
    };
} // namespace

bool SemanticAnalyzer::Analysis(AST* tree)
{
    if (!tree)
    {
        return false;
    }

    DeclarationChecker checker;
    tree->Accept(checker);
    return checker.HasError();
}
