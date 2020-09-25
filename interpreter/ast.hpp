#ifndef AST_HPP
#define AST_HPP

#include "local.hpp"

struct AST_Node {
	enum class Type { BinExp, IntegerExp } tag; // Holds information about the type of expression this is.
	union {

		SemanticInfo value;

		struct {
			char op;
			struct AST_Node* left;
			struct AST_Node* right;
		} BinExp;

		struct {
			char op;
			struct AST_Node* operand;
		} UnExp;

		struct {
			const char* name;
			struct AST_List* args;
		} CallExp;

	} operation;
};

typedef struct AST_List {
	AST_Node* element;
	struct AST_List* next;
};

// Literals
AST_Node* IntegerExp(int i);

// Binary Expressions

AST_Node* BinaryExp(char op, AST_Node* left, AST_Node* right);

#endif