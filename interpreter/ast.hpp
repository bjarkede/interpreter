#ifndef AST_HPP
#define AST_HPP

#include "local.hpp"

enum BinaryOpType {
	// Arithmetic
	Add, Sub, Mul, Mod, Pow,
	Div, IDiv,
	// Comparison
	EQ, LT,
	// Logical
	And, Or,
	// Others
	MIN, MAX,

	NoBinOp,
	OP_COUNT
};

enum UnOpType {
	Not,
	Minus,
	BNot,
	Len,
	NoUnOp
};

enum ExpressionType
{
	E_Integer,
	E_String,
	E_BinOp,
	E_UnOp,
	E_NoType,
	E_Count
};

typedef struct Expression {
	ExpressionType expType;
} Exp;

typedef struct Integer : public Expression {
	unsigned long long value;
} Integer;

typedef struct String : public Expression {
	const char* value;
} String;

typedef struct BinOp : public Expression {
	BinaryOpType opType;
	Expression* left;
	Expression* right;
} BinOp;

typedef struct UnOp : public Expression {
	UnOpType opType;
	Expression* expr;
} UnOp;

// Empty
Expression* EmptyExp();

// Literals
Expression* IntegerExp(L_INTEGER i);
Expression* StringExp(L_STRING* val);

// Binary Expressions
Expression* BinaryExp(BinaryOpType op, Expression* left, Expression* right);

// Unary
Expression* UnaryExp(UnOpType op, Expression* expr);

#endif