# Presentation

This is a work in progress interpreter that implements its own lexical- and syntax analysis, along with type-checking. The goal is to directly process the syntax tree to evaluate expressions, statements and so on. Furthermore there is a focus on making interpretation faster, and providing precise and informative error messages.

## Requirements

* Windows x64
* GNU Make or Visual Studio c++ 2013 or later for compiling

## Project breakdown

| Sub-project | Purpose |
| :--|:--|
| Error messages | The interpreter outputs precise and informative error messages. |
| Optimized interpreter | The interpreter is optimized to combat slower execution times compared to a compiler. |
| Lexical analysis | The interpreter uses a self-coded lexer to read and divide text into tokens. |
| Syntax analysis | The interpreter uses a self-coded parser that takes the output from the lexing and produce a syntax tree. |
| Type checking | The syntax tree optained from the parsing stage is analyzed to determine if the program violates consistency requirements. |

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
