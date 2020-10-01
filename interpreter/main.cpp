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

    Parse(ls);

    PrintDebug("Finished lexing, parsing and interpretingvalidating file: %s\n", argv[1]);

    return 1;
}