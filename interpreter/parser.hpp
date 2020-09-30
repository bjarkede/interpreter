#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "local.hpp"
#include "lexer.hpp"

#define MAXSIZE 100

#include <stack>
#include <list>
#include <map>

// Table-driven LL(1) parsing using stack automata
void LL1(LexerState* ls);

// LL(1) Recursive-descent parsing
void Parse(LexerState* ls);
void PrimaryExpression(LexerState* ls);
void SuffixedExpression(LexerState* ls);
void SecondaryExpression(LexerState* ls);
void SuffixedSecondaryExpression(LexerState* ls);
void SimpleExpression(LexerState* ls);

// Test Parsing
Exp* ParseExpression(LexerState* ls);
Exp* ParseExpressionTernary(LexerState* ls);
Exp* ParseParenthesisedExpression(LexerState* ls);
Exp* ParseExpressionOr(LexerState* ls);
Exp* ParseExpressionOperand(LexerState* ls);
Exp* ParseExpressionAnd(LexerState* ls);
Exp* ParseExpressionCompare(LexerState* ls);
Exp* ParseExpressionAdd(LexerState* ls);
Exp* ParseExpressionMul(LexerState* ls);
Exp* ParseExpressionUnary(LexerState* ls);
Exp* ParseExpressionBase(LexerState* ls);

// Helpers for Table-driven LL(1)
std::list<Exp*> makeNodes(std::list<int> ListOfGrammars);
std::list<int> rightHandSide(int tableIndex);

// Helpers in general.
void check_match(LexerState* ls, int what, int who, int where);
bool test_next(LexerState* ls, int c);

void pushrule(const char* s);
void poprule(void);

BinaryOpType GetBinaryOperator(int op);
UnOpType GetUnaryOperator(int op);
bool IsCompOperator(LexerState* ls);
bool IsAddOperator(LexerState* ls);
bool IsMulOperator(LexerState* ls);
bool IsUnaryOperator(LexerState* ls);

#endif