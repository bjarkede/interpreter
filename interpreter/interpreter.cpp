#include "interpreter.hpp"

Value eval(AST_Node* Expr) {
	switch (Expr->tag) {
	case AST_Node::Type::IntegerExp: 
	{
		Value result;
		result.v.i = Expr->operation.value.i;
		return result;
	} break;
	case AST_Node::Type::BinExp: 
	{
		Value result;
	
		Value lvalue = eval(Expr->operation.BinExp.left);
		Value rvalue = eval(Expr->operation.BinExp.right);

		switch (Expr->operation.BinExp.op) {
		case '+': 
		{
			result.v.i = lvalue.v.i + rvalue.v.i;
			return result;
		} break;
		case '-':
		{
			result.v.i = lvalue.v.i - rvalue.v.i;
			return result;
		} break;
		case '*':
		{
			result.v.i = lvalue.v.i * rvalue.v.i;
			return result;
		} break;
		}
	} break;
	}

}

std::string toString(AST_Node* Expr) {

	std::ostringstream buffer;

	switch (Expr->tag) {
	case AST_Node::Type::IntegerExp: 
	{
		buffer << Expr->operation.value.i;
		return buffer.str();
	} break;
	case AST_Node::Type::BinExp: {
		buffer << toString(Expr->operation.BinExp.left) << Expr->operation.BinExp.op << toString(Expr->operation.BinExp.right);
		return buffer.str();
	} break;
	}
}