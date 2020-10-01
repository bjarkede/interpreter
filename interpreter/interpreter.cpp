#include "interpreter.hpp"

struct cmp_str
{
	bool operator()(char const* a, char const* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

static std::map<const char*, Value, cmp_str> valueenv;

Value lookup(const char* name) {
	auto it = valueenv.find(name);
	if (it != valueenv.end())
		return it->second;

	FatalError("InterpretationError: Couldn't find variable [%s] in value environment.", name);
}

Value eval(Expression* e) {
	
	Value v;
	
	switch (e->expType) {
	case E_Integer:
	{
		v.v.i = ((Integer*)e)->value;
		return v;
	} break;
	case E_Variable:
	{
		v = lookup(((Var*)e)->name);
		return v;
	} break;
	case E_LetBinding: {
		Value xval = eval(((Let*)e)->binding);
		valueenv.insert(std::pair<const char*, Value>(((Var*)((Let*)e)->variable)->name, xval));

		v = eval(((Let*)e)->expr);
		return v;
	} break;
	case E_BinOp:
	{

		Value lvalue = eval(((BinOp*)e)->left);
		Value rvalue = eval(((BinOp*)e)->right);

		switch (((BinOp*)e)->opType) {
		case Add:
		{
			v.v.i = lvalue.v.i + rvalue.v.i;
			return v;
		} break;
		case Sub:
		{
			v.v.i = lvalue.v.i - rvalue.v.i;
			return v;
		} break;
		case Mul:
		{
			v.v.i = lvalue.v.i * rvalue.v.i;
			return v;
		} break;
		case Div:
		{
			v.v.i = lvalue.v.i / rvalue.v.i;
			return v;
		}
		}
	} break;
	case E_Paren: {
		v = eval(((Paren*)e)->expr);
		return v;
	} break;
	}
}

std::string toString(Expression* e) {

	static const char* operands[] = { "+", "-", "*", "%", "^", "/" };

	std::ostringstream buffer;

	switch (e->expType) {
	case E_Integer: 
	{
		buffer << ((Integer*)e)->value;
	} break;
	case E_Variable: {
		buffer << ((Var*)e)->name;
	} break;
	case E_BinOp: {
		buffer << toString(((BinOp*)e)->left) << operands[((BinOp*)e)->opType] << toString(((BinOp*)e)->right);
	} break;
	case E_Paren: {
		buffer << "(" << toString(((Paren*)e)->expr) << ")";
	} break;
	case E_LetBinding: {
		buffer << "let " << toString(((Let*)e)->variable) << " = " << toString(((Let*)e)->binding) << " in " << toString(((Let*)e)->expr) << " end";
	} break;
	}

	return buffer.str();
}