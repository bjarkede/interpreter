# Presentation

This is a work in progress interpreter that implements its own lexical- and syntaxanalysis, along with type-checking. The goal is to directly process the syntax tree to evaluate expressions, statements and so on.

The goal is to try and make interpretation faster, and provide precise and informative error messages.

## Requirements

* Windows x64
* GNU Make or Visual Studio c++ 2013 or later for compiling

## Project breakdown

| Sub-project | Purpose | Status |
| :--|:--|:--|
| Error messages | The interpreter outputs precise and informative error messages. | Not started |
| Optimized interpreter | The interpreter is optimized to combat slower execution times compared to a compiler. | In progress |
| Lexical analysis | The interpreter uses a self-coded lexer to read and divide text into tokens. | In progress |
| Syntax analysis | The interpreter uses a self-coded parser that takes the output from the lexing and produce a syntax tree. | Not started |
| Type checking | The syntax tree optained from the parsing stage is analyzed to determine if the program violates consistency requirements. | Not started |

| Feature | Support |
| :--|:--|
| Binary operations | YES |
| Let-binding and sequential let-bindings | YES |
| Variable substitution | YES |
| Free variables | YES |
| First- and higher order functions | YES |
| Type checking | T.B.D |
| Tuples and selectors | YES |

## References

# Licenses