#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "symtable.hpp"

symtable<Value*> valueenv;

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

	//PrintDebug("SyntaxMessage: Input accepted by parser.\n\n");
	//PrintDebug("Testing AST Interpretation:\n");

	valueenv.empty(701);

	for (auto& e : expList) {
		PrintDebug("Expression: [ %s ]\n", toString(e).c_str());
		auto v = eval(e, valueenv);
		if (v != nullptr) {
			switch (v->vType) {
			case V_Integer: { PrintDebug("Value: %d\n\n", ((IntVal*)v)->i); } break;
			default:
			{}
			}
		}
	}

    PrintDebug("Finished lexing, parsing and interpreting file: %s\n", argv[1]);

	
    return 1;
}