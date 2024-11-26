#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class NullCheckHandler : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) {
        if (const BinaryOperator *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("nullCheck")) {
            llvm::outs() << "NULL check found at "
                         << BinOp->getBeginLoc().printToString(*Result.SourceManager) << "\n";
        }

        if (const MemberExpr *MemberAccess = Result.Nodes.getNodeAs<MemberExpr>("unsafeDereference")) {
            const auto *Parent = Result.Context->getParents(*MemberAccess).begin();
            if (Parent != Result.Context->getParents(*MemberAccess).end()) {
                if (Parent->get<IfStmt>()) {
                    // Safely guarded by if-statement
                    return;
                }
            }
            llvm::outs() << "Potential NULL pointer dereference at "
                         << MemberAccess->getBeginLoc().printToString(*Result.SourceManager) << "\n";
        }
    }
};

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, llvm::cl::GeneralCategory);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    NullCheckHandler Handler;
    MatchFinder Finder;

    // Match NULL checks
    Finder.addMatcher(
        binaryOperator(
            hasOperatorName("=="),
            hasEitherOperand(implicitCastExpr(hasSourceExpression(nullPointerConstant())))
        ).bind("nullCheck"),
        &Handler
    );

    // Match pointer dereferences without NULL checks
    Finder.addMatcher(
        memberExpr(
            hasObjectExpression(ignoringParenCasts(declRefExpr(to(varDecl()))))
        ).bind("unsafeDereference"),
        &Handler
    );

    return Tool.run(newFrontendActionFactory(&Finder).get());
}
