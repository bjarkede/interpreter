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

	PrintDebug("SyntaxMessage: Input accepted by parser.\n\n");
	PrintDebug("Testing AST Interpretation:\n");

	for (auto& e : expList) {
		PrintDebug("Expression: [ %s ]\n", toString(e).c_str());
		auto v = eval(e, valueenv);
		switch (v->vType) {
		case V_Integer: { PrintDebug("Value: %d\n\n", ((IntVal*)v)->i); }
		}
	}

    PrintDebug("Finished lexing, parsing and interpreting file: %s\n", argv[1]);
	
	/*
	symtable<Value> sTable;
	Value v;
	v.v.i = 10;
	sTable.bind(v, "xyz");
	auto x = sTable.lookup("xyz");
	v.v.i = 11;
	sTable.bind(v, "xyz");
	sTable.enter();
	sTable.bind(v, "yzx");
	sTable.bind(v, "zxy");
	sTable.bind(v, "yxz");
	sTable.exit();
	*/


    return 1;
}