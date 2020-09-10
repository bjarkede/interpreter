#ifndef LOCAL_HPP
#define LOCAL_HPP

#include "shared.hpp"

enum RESERVED {
	TK_LET, TK_IF, TK_THEN, TK_ELSE, TK_IN, TK_END,
	TK_TRUE, TK_FALSE, TK_NAME,
	TK_EQ, TK_AND, TK_OR, TK_LE, TK_NOT,
	TK_MIN, TK_MAX, TK_EOZ, TK_INT,
	TK_COUNT
};

#define NUM_RESERVED ((int)(TK_COUNT));

typedef union {
} SemanticInfo;

typedef struct Token {
	int token;
	int line, col;
	void* data;
	// TODO: Add some semantic info?
} Token;

typedef struct LexerState {
	u8 currentChar; // Current charint
	int lineNumber;  // Current linenumber (gets incremented when /n/r)
	int lastLine;    // Line of the last consumed token.

	Token t;
	Token lookahead;

	Buffer IOZ; // Character sequence stream.
	u8* z; // Pointer to the current character in the input stream.
	Buffer TBuffer; // Buffer for tokens.
} LexerState;


#endif