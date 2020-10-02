#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "symtable.hpp"

// @Debug:
// We use this main to test the functions used in the lexer.
int main(int argc, char** argv) {

    LexerState* ls = (LexerState*)malloc(sizeof(LexerState));

    if (!InitializeLexer(argv[1], ls)) {
        FatalError("Couldn't process input stream: %s\n", argv[1]);
    }

	std::vector<Expression*> expList;
    
	ProcessNextToken(ls);
	while (ls->t.token != TK_EOZ) {
		expList.push_back(ParseExpression(ls));
	}

	PrintDebug("SyntaxMessage: Input accepted by parser.\n\n");
	PrintDebug("Testing AST Interpretation:\n");

	for (auto& e : expList) {
		PrintDebug("Expression: [ %s ]\n       Value: %d\n\n", toString(e).c_str(), eval(e).v);
	}

    PrintDebug("Finished lexing, parsing and interpreting file: %s\n", argv[1]);

	symtable<Value> sTable;
	Value v;
	v.v.i = 10;
	sTable.bind(v, "xyz");
	auto x = sTable.lookup("xyz");



    return 1;
}