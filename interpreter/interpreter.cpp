#include "interpreter.hpp"

Value* lookup(const char* name, symtable<Value*> env) {
	Value* result = env.lookup(name);
	if (result->vType == V_NoType) FatalError("InterpreterError: Couldn't find variable [%s] in value environment.", name);
	return result;
}

Value* eval(Expression* e, symtable<Value*> env) {
	switch (e->expType) {
	case E_Integer:
	{
		return MakeIntegerVal((((Integer*)e)->value));
	} break;
	case E_Variable:
	{
		return lookup(((Var*)e)->name, env);
	} break;
	case E_LetBinding: {
		Value* xval = eval(((Let*)e)->binding, env);
		env.enter();
		env.bind(xval, ((Var*)((Let*)e)->variable)->name);
		Value* returnval = eval(((Let*)e)->expr, env); // Evaluate the expression in-scope.
		env.exit();
		return returnval;
	} break;
	case E_LetFun: {
		symtable<Value*> bodyenv = env;
		bodyenv.bind(MakeClosureVal((((LetFun*)e)->f), (((LetFun*)e)->x), (((LetFun*)e)->fbody), env),(((LetFun*)e)->f));
		return eval(((LetFun*)e)->letbody, bodyenv);
	} break;
	case E_Call: {
		auto fClosure = eval(((Call*)e)->eFun, env);
		if (fClosure->vType == V_Closure) {
			symtable<Value*> fbodyenv = ((Closure*)fClosure)->fdeclenv;
			Buffer varNames = ((Closure*)fClosure)->x;
			// Evaluate each expression in the function call arguments and
			// bind them to the function body.
			for (int i = 0; i < ((Call*)e)->args.writeIndex; i++) {
				fbodyenv.bind(eval(((Expression**)((Call*)e)->args.buffer)[i], env), ((Var*)((Exp**)varNames.buffer)[i])->name);
			}
			return eval(((Closure*)fClosure)->fbody, fbodyenv); // Now we can evaluate the function.
		}
	} break;
	case E_BinOp:
	{
		Value* lvalue = eval(((BinOp*)e)->left, env);
		Value* rvalue = eval(((BinOp*)e)->right, env);

		switch (((BinOp*)e)->opType) {
		case Add:
		{
			switch (lvalue->vType) {
			case V_Integer:
			{
				switch (rvalue->vType) {
				case V_Integer: { return MakeIntegerVal(((IntVal*)lvalue)->i + ((IntVal*)rvalue)->i); }
				case V_Float: { return MakeFloatVal(((IntVal*)lvalue)->i + ((FloatVal*)rvalue)->f); }
				}
			} break;
			case V_Float: {
				switch (rvalue->vType) {
				case V_Integer: { return MakeIntegerVal(((FloatVal*)lvalue)->f + ((IntVal*)rvalue)->i); }
				case V_Float: { return MakeFloatVal(((FloatVal*)lvalue)->f + ((FloatVal*)rvalue)->f); }
				}
			}
			}
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
		return eval(((Paren*)e)->expr, env);
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
		Exp** x = (Exp**)((Call*)e)->args.buffer;

		buffer << "let " << (((LetFun*)e)->f) << "(";;
		if (((Call*)e)->args.writeIndex > 1) { // If there's more than one variable
			for (int i = 0; i < ((Call*)e)->args.writeIndex - 1; i++) {
				buffer << toString(x[i]) << ",";
			}
			buffer << toString(x[((Call*)e)->args.writeIndex-1]);
		}
		else {
			buffer << toString(x[0]);
		}
		buffer << ") = " << toString(((LetFun*)e)->fbody) << " in " << toString(((LetFun*)e)->letbody) << " end";
	} break;
	case E_Call: {
		Exp** x = (Exp**)((Call*)e)->args.buffer;

		buffer << toString(((Call*)e)->eFun) << "(";
		if (((Call*)e)->args.writeIndex > 1) {
			for (int i = 0; i < ((Call*)e)->args.writeIndex - 1; i++) {
				buffer << toString(x[i]) << ",";
			}
			buffer << toString(x[((Call*)e)->args.writeIndex - 1]);
		}
		else {
			buffer << toString(x[0]);
		}
		buffer << ")";
	} break;
	default:
		buffer << "";
	}

	return buffer.str();
}

Value* MakeEmptyVal() {
	Value* v = (Value*)malloc(sizeof(Value));
	v->vType = V_NoType;
	return v;
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

Value* MakeFloatVal(double f) {
	FloatVal* v = (FloatVal*)malloc(sizeof(FloatVal));
	v->vType = V_Float;
	v->f = f;
	return v;
}

Value* MakeClosureVal(const char* f,
	Buffer x,
	Expression* fbody,
	symtable<Value*> fdeclenv) {
	Closure* v = new Closure;
	v->vType = V_Closure;
	v->f = f;
	v->x = x;
	v->fbody = fbody;
	v->fdeclenv = fdeclenv;
	return v;
}