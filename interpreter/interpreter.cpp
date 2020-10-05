#include "interpreter.hpp"


// @TODO:
// symbol tabels, a bunch of expressions. etc.

struct cmp_str
{
	bool operator()(char const* a, char const* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};
 
//static std::map<const char*, Value*, cmp_str> valueenv;
static symtable<Value*> valueenv;

Value* lookup(const char* name) {
	Value* result = valueenv.lookup(name);
	if (result->vType == V_NoType) FatalError("InterpreterError: Couldn't find variable [%s] in value environment.", name);
	return result;
	/*auto it = valueenv.find(name);
	if (it != valueenv.end())
		return it->second;*/

	FatalError("InterpreterError: Couldn't find variable [%s] in value environment.", name);
}

Value* eval(Expression* e) {
		
	switch (e->expType) {
	case E_Integer:
	{
		return MakeIntegerVal((((Integer*)e)->value));
	} break;
	case E_Variable:
	{
		return lookup(((Var*)e)->name);
	} break;
	case E_LetBinding: {
		Value* xval = eval(((Let*)e)->binding);
		valueenv.bind(xval, ((Var*)((Let*)e)->variable)->name);
		return eval(((Let*)e)->expr);
	} break;
	case E_LetFun: {
		auto bodyenv = valueenv;
		bodyenv.bind(MakeClosureVal((((LetFun*)e)->f), (((LetFun*)e)->x), (((LetFun*)e)->fbody), valueenv),(((LetFun*)e)->f));
		return eval(((LetFun*)e)->letbody);
	} break;
	case E_BinOp:
	{
		Value* lvalue = eval(((BinOp*)e)->left);
		Value* rvalue = eval(((BinOp*)e)->right);

		switch (((BinOp*)e)->opType) {
		case Add:
		{
			return MakeIntegerVal(((IntVal*)lvalue)->i + ((IntVal*)rvalue)->i);
		} break;
		case Sub:
		{
			return MakeIntegerVal(((IntVal*)lvalue)->i - ((IntVal*)rvalue)->i);
		} break;
		case Mul:
		{
			return MakeIntegerVal(((IntVal*)lvalue)->i * ((IntVal*)rvalue)->i);
		} break;
		case Div:
		{
			return MakeIntegerVal(((IntVal*)lvalue)->i / ((IntVal*)rvalue)->i);
		}
		}
	} break;
	case E_Paren: {
		return eval(((Paren*)e)->expr);
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
	case E_LetFun: {
		buffer << "let " << (((LetFun*)e)->f) << "(";;

		buffer << ") = " << toString(((LetFun*)e)->fbody) << " in " << toString(((LetFun*)e)->letbody) << " end";
	} break;
	}

	return buffer.str();
}

Value* MakeIntegerVal(L_INTEGER i) {
	IntVal* v = (IntVal*)malloc(sizeof(IntVal));
	v->vType = V_Integer;
	v->i = i;
	return v;
}

Value* MakeStringVal(L_STRING s) {
	StringVal* v = (StringVal*)malloc(sizeof(StringVal));
	v->vType = V_String;
	v->s.contents = s.contents;
	v->s.flags = s.flags;
	v->s.length = s.length;
	v->s.u = s.u;
	return v;
}

Value* MakeClosureVal(const char* f,
	char** x,
	Expression* fbody,
	symtable<Value*> fdeclenv) {
	Closure* v = (Closure*)malloc(sizeof(Closure));
	v->vType = V_Closure;
	v->f = f;
	v->x = x;
	v->fbody = fbody;
	v->fdeclenv = fdeclenv;
	return v;
}