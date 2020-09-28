#include "ast.hpp"

#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

// @Debug:
// We use this main to test the functions used in the lexer.
int main(int argc, char** argv) {

    LexerState* ls = (LexerState*)malloc(sizeof(LexerState));

    if (!InitializeLexer(argv[1], ls)) {
        FatalError("Couldn't process input stream: %s\n", argv[1]);
    }

    //LL1(ls);

    Parse(ls);

    // @Note:
    // All code below is for testing purposes.
   /* PrintDebug("Tokens:\n");
    for (;;) {
        ProcessNextToken(ls);

        if (ls->t.token == TK_INT) {
            printf("Type: %d, Line: %d[%d ], Integer: [ %d ]\n", ls->t.token,
                ls->lineNumber,
                ls->t.col, ls->t.semInfo.i);
        }
        else {
            printf("Type: %d, Line: %d[%d ], Symbol: [ %s ]\n", ls->t.token,
                ls->lineNumber,
                ls->t.col,
                (ls->t.semInfo.s == NULL ? (char*)&ls->t.token : ls->t.semInfo.s->contents));
        }

        if (ls->t.token == TK_EOZ) return true;
    }
    */

    PrintDebug("Finished lexing and parse validating file: %s\n", argv[1]);
    PrintDebug("Testing AST Interpretation:\n");

    Exp* e = BinaryExp(Sub, BinaryExp(Mul, IntegerExp(5), IntegerExp(3)), IntegerExp(5));

    PrintDebug("Expression [ %s ] evaluates to: %d\n", toString(e).c_str(), eval(e).v.i);

    return 1;
}