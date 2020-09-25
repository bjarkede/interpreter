#ifndef PARSER_HPP
#define PARSER_HPP

void Expression(LexerState* ls);
void PrimaryExpression(LexerState* ls);
void SubExpression(LexerState* ls, int limit);
void SimpleExpression(LexerState* ls);
void SuffixedExpression(LexerState* ls);

// Terminals
void SingleVariable(LexerState* ls);

// Helpers

static void check_match(LexerState* ls, int what, int who, int where);
static bool test_next(LexerState* ls, int c);

#endif