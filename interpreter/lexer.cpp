#include "local.hpp"
#include "lexer.hpp"

static bool InitializeLexerState(LexerState* ls, int firstChar) {
    ls->t = {};
    ls->lookahead = {};
    
    ls->currentChar = firstChar;
    ls->lookahead.token = TK_EOZ;

    ls->lineNumber = ls->lastLine = 1;

    ls->z = (u8*)(ls->IOZ.buffer);
    if (!AllocBuffer(ls->TBuffer, MINBUFFER)) {
        FatalError("Couldn't allocate %d bytes for token buffer.", MINBUFFER);
        return false;
    }

    return true;
}

static bool ProcessCharacterSequence(char* filePath, LexerState* ls) {
    if (ReadEntireFile(ls->IOZ, filePath)) {
        if (InitializeLexerState(ls, ((char*)ls->IOZ.buffer)[0])) {
            for (;;) {
                ProcessNextToken(ls);
                if (ls->t.token == TK_EOZ) return true;
            }
        }
    }
    else {
        return false;
    }
    return true;
}

static void ProcessNextToken(LexerState* ls) {
    ls->lastLine = ls->lineNumber;
    if (ls->lookahead.token != TK_EOZ) {
        ls->t = ls->lookahead;
        ls->lookahead.token = TK_EOZ;
    }
    else {
        ls->t.token = Lex(ls);

        switch (ls->t.token) {
        case TK_INT: {
            ls->t.line = ls->lineNumber;
        } break;
        case TK_EOZ: {
            return;
        }
        default:
            break;
        }

        reset_buffer(ls);
        
        printf("Token ID: %d, Line: %d[%d], Data: %d\n", ls->t.token, ls->lineNumber, ls->t.col,(int)ls->t.data);
    }
}

static int Lex(LexerState* ls) {
    //ls->TBuffer.buffer = 0; // Maybe add more buffer functions to the struct.
    ls->t.data = 0;
    for (;;) {
        switch (ls->currentChar) {
        case '\n': case '\r': {
            InclineLineNumber(ls);
        } break;
        case ' ': case '\f': case '\t': case '\v': {
            next(ls);
        } break;
        case '\0': {
            return TK_EOZ;
        } break;
        case '+': {
            save_and_next(ls);
            if (!isdigit(ls->currentChar)) return  '+';
            return ReadNumeralLiteral(ls);
        } break;
        case '-': { 
            save_and_next(ls);
            if (!isdigit(ls->currentChar)) return  '-';
            return ReadNumeralLiteral(ls);
        } break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            return ReadNumeralLiteral(ls);
        } break;
        default:
            int c = ls->currentChar;
            next(ls);
            return c;
        }
    }
}

// TODO:
static int ReadNumeralLiteral(LexerState* ls) {
    int first = ls->currentChar;
    int value = 0;

    assert(isdigit(first));
    save_and_next(ls);
    for (;;) {
        if (isdigit(ls->currentChar))
            save_and_next(ls);
        else break;
    }

    SaveToken(ls, '\0'); // add null to indicate end of sequence.

    if (!StringToNumber(ls, &value)) FatalError("Failed when converting string to number.");
    
    ls->t.data = (void*)value;
    return TK_INT;
}

static bool StringToNumber(LexerState* ls, int* value) {

    unsigned int result = 0;
    int negative;

    char* buff = (char*)ls->TBuffer.buffer;

    if (*buff == '-') { buff++; negative = 1; }
    else if (*buff == '+') { buff++; negative = 0;} else { negative = 0; }

    for (;isdigit((char)*buff);buff++) {
        int d = *buff - '0';
        result = result * 10 + d;
    }

    *value = (negative ? 0u - result : result);

    return true;
}

static void SaveToken(LexerState* ls, int c) {
    // if we are about to write out of memory
    if (ls->TBuffer.writeIndex + 1 > ls->TBuffer.length) {
        if (ls->TBuffer.old_buffer) delete[] ls->TBuffer.old_buffer;

        void* new_buffer = 0;
        new_buffer = malloc(ls->TBuffer.length * 2);
        if (new_buffer == 0) FatalError("Failed to allocate buffer when saving.");
        if (ls->TBuffer.buffer) {
            memcpy(new_buffer, ls->TBuffer.buffer, ls->TBuffer.length);
        }

        ls->TBuffer.old_buffer = ls->TBuffer.buffer;
        ls->TBuffer.buffer = new_buffer;
    }

    ls->TBuffer.old_buffer = ls->TBuffer.buffer;
    ((char*)ls->TBuffer.buffer)[ls->TBuffer.writeIndex++] = (char)c;
}

static void InclineLineNumber(LexerState* ls) {
    int old = ls->currentChar;
    assert(ls->currentChar == '\n' || ls->currentChar == '\r');
    next(ls);
    if ((ls->currentChar == '\n' || ls->currentChar == '\r') && ls->currentChar != old)
        next(ls);
    if (++ls->lineNumber >= INT64_MAX)
        FatalError("Read too many lines in current chunk. Integer overflow.\n");

    ls->t.col = 0;
}

// @Debug:
// We use this main to test the functions used in the lexer.
int main(int argc, char** argv) {

    LexerState* ls = (LexerState*)malloc(sizeof(LexerState));

    if (!ProcessCharacterSequence(argv[1], ls)) {
        FatalError("Couldn't process input stream: %s\n", argv[1]);
    }

    PrintDebug("Finished lexing file: %s\n", argv[1]);

    return 1;
}