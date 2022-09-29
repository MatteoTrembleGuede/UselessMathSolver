#pragma once

#include <string>
#include <sstream>

class Expression
{

	enum Operation
	{
		Add,
		Sub,
		Mul,
		Div,
		Pow,
		Num,
		Invalid
	};

private:

	bool solved = false;
	bool log = false;
	std::string expr;
	std::string firstTerm;
	std::string secondTerm;
	std::string res;

	Operation op;

	typedef std::string (Expression::*OperationFct) (std::string t1, std::string t2);
	std::string Addition(std::string t1, std::string t2);
	std::string Subtract(std::string t1, std::string t2);
	std::string Multiply(std::string t1, std::string t2);
	std::string Divide(std::string t1, std::string t2);
	Expression GetRationnal(std::string t1);
	std::string Power(std::string t1, std::string t2);
	std::string NaturalPower(std::string t1, std::string t2);
	std::string Root(std::string t1, std::string t2);
	std::string EuclideanDiv(std::string t1, std::string t2);
	std::string GetOperation();

	void ReduceExpr();
	void ReduceFraction(std::string& t1, std::string& t2);
	void RemoveUselessBrackets();
	void RemoveSpaces();
	bool CheckIsNumber();
	bool CheckIsNumber(std::string a);
	void Solve(bool _log = true);

	std::stringstream fakeBuf;

public:

	Expression(std::string _exp) : expr(_exp) {};
	Expression(const Expression& _exp) : expr(_exp.expr) {};
	Expression operator=(const Expression& _exp) {
		Expression exp(_exp);
		exp.solved = false;
		return exp;
	};
	operator std::string();

};

