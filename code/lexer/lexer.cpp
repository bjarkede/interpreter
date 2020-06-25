#include "local.hpp"

struct Token {
  int line;
  int col;
  token_t type;
  void* data;
};

// @TODO:
// We need to process the character sequence and generate tokens.
// There's a couple of ways to do this, but I think we should try using regular expressions. -bjarke, 25th June 2020
static bool ProcessCharacterSequence(char* filePath) {

  Buffer buf;
  
  if(ReadEntireFile(buf, filePath)) {
    // @Debug:
    // See if we read a file correctly.
    for(int i = 0; i < buf.length; i++) {
      printf("%c", ((char*)buf.buffer)[i]);
    }
    
  } else {
    return false;
  }

  return true;
}

// @Debug:
// We use this main to test the functions used in the lexer.
int main(int argc, char** argv) {
   
  if(!ProcessCharacterSequence(argv[1])) {
    printf("Error.\n");
  }

  return 1;
}

