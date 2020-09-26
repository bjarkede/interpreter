#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "local.hpp"

// @Nocheckin
#include <string>
#include <sstream>
#include <iostream>

typedef struct Value {
	enum class type { Integer, String, Bool };
	union
	{
		L_INTEGER i;
		L_STRING s;
		L_BOOL l;
	} v;
} Value;

Value eval(Expression* Expr);
std::string toString(Expression* Expr);

#endif