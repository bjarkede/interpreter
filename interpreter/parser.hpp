#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "local.hpp"
#include "lexer.hpp"

#define MAXSIZE 100

#include <stack>
#include <map>

typedef enum NonTerminal {
	NT_EXP,
	NT_EXP_,
	NT_EXP2,
	NT_EXP2_,
	NT_EXP3,
	NT_COUNT
} NTerm;

// Table-driven LL(1) parsing using stack automata
void LL1(LexerState* ls);

// LL(1) Recursive-descent parsing
void Parse(LexerState* ls);
void PrimaryExpression(LexerState* ls);
void SuffixedExpression(LexerState* ls);
void SecondaryExpression(LexerState* ls);
void SuffixedSecondaryExpression(LexerState* ls);
void SimpleExpression(LexerState* ls);

Exp* TerminalNode(RESERVED terminal);
Exp* NonTerminalNode(RESERVED nonterminal, Exp* subtrees);

// Helpers for Table-driven LL(1)
Exp* makeNodes(Exp* e, int* arr);
int* rightHandSide(int tableIndex);

// Helpers in general.
void check_match(LexerState* ls, int what, int who, int where);
bool test_next(LexerState* ls, int c);

void pushrule(const char* s);
void poprule(void);

#endif