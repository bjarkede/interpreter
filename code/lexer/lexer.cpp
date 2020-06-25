// Lexer

#include "local.hpp"

struct Token {
  int line;
  int col;
  token_t type;
  void* data;
};

static bool ProcessCharacterSequence(char* filePath) {

  Buffer buf;
  
  if(ReadEntireFile(buf, filePath)) {
    printf("%s", buf.buffer);
  } else {
    return false;
  }

  return true;
}

int main(int argc, char** argv) {
   
  if(!ProcessCharacterSequence(argv[1])) {
    printf("Error.\n");
  }

  return 1;
}

