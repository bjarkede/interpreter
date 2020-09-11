#ifndef LEXER_HPP
#define LEXER_HPP

#define next(ls) (ls->currentChar = *(++ls->z), ls->t.col++)
#define save_and_next(ls) (SaveToken(ls, ls->currentChar),next(ls))
#define reset_buffer(ls) (memset(ls->TBuffer.buffer, 0, ls->TBuffer.length), ls->TBuffer.writeIndex = 0)

bool ProcessCharacterSequence(char* filePath, LexerState* ls);
bool InitializeLexerState(LexerState* ls, int firstChar);

int Lex(LexerState* ls, SemanticInfo* semInfo);
int ReadNumeralLiteral(LexerState* ls, SemanticInfo* semInfo);

void InclineLineNumber(LexerState* ls);

void SaveToken(LexerState* ls, int c);
void ProcessNextToken(LexerState* ls);

static bool StringToNumber(LexerState* ls, int* value);
L_STRING* CreateVariableString(LexerState* ls, const char* str, size_t l);

#endif