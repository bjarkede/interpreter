#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "local.hpp"

// @Nocheckin
#include <string>
#include <sstream>
#include <iostream>
#include <map>

typedef struct Value {
	enum class type { Integer, String, Bool };
	union
	{
		L_INTEGER i;
		L_STRING s;
		L_BOOL l;
	} v;
} Value;

// Interpreting
Value eval(Expression* Expr);

// Lookup-functions
// int lookup(const char* varName);
// Value lookup(const char* varName);
Value lookup(const char* name);

// Utility
std::string toString(Expression* Expr);


#endif