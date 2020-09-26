#include "interpreter.hpp"

Value eval(Expression* e) {
	
	Value v;
	
	switch (e->expType) {
	case E_Integer:
	{
		v.v.i = ((Integer*)e)->value;
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
		}
	} break;
	}
}

std::string toString(Expression* e) {

	const char* operands[] = { "+", "*", "-" };

	std::ostringstream buffer;

	switch (e->expType) {
	case E_Integer: 
	{
		buffer << ((Integer*)e)->value;
		return buffer.str();
	} break;
	case E_BinOp: {
		buffer << toString(((BinOp*)e)->left) << operands[((BinOp*)e)->opType] << toString(((BinOp*)e)->right);
		return buffer.str();
	} break;
	}
}