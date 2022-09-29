#include "Expression.h"
#include <iostream>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))
#define assert(b) { if (!(b)) throw "assertion failed"; }

#define Cout (log ? std::cout : fakeBuf)

char CharAddition(char c1, char c2, char& _hold)
{
	char i1 = c1 - 48;
	char i2 = c2 - 48;
	char h = _hold - 48;

	_hold = ((i1 + i2 + h) / 10) + 48;
	char res = ((i1 + i2 + h) % 10) + 48;
	return res;
}

int GetComma(std::string t)
{
	int c = 0;
	for (int i = 0; i < t.size(); ++i)
	{
		int id = t.size() - i - 1;
		if (t[id] == '.' || t[id] == ',')
		{
			c = i;
			break;
		}
	}
	return c;
}

void FillToSameSize(std::string& a, std::string& b)
{
	if (a[a.size() - 1] == '\0')
	{
		a = std::string(a.c_str(), a.size() - 1);
	}
	std::string& shortest = (a.size() < b.size() ? a : b);
	std::string& longest = (a.size() > b.size() ? a : b);
	for (int i = 0; shortest.size() < longest.size(); ++i) shortest = std::string("0") + shortest;
}

void RemoveComma(std::string& a)
{
	int i = GetComma(a);
	if (i == 0)
	{
		FillToSameSize(a, a);
		if (a[a.size() - 1] == '.' || a[a.size() - 1] == ',')
		{
			a = std::string(a.c_str(), a.size() - 1);
		}
		return;
	}
	a = std::string(a.c_str(), a.size() - i - 1) + std::string(a.c_str() + a.size() - i, i);
}

void MulPowerOfTen(std::string& t1, int p)
{
	if (p == 0) return;
	int c1 = GetComma(t1);
	RemoveComma(t1);
	p = c1 - p;

	if (p < 0)
	{
		while (-p > 0)
		{
			t1 += "0";
			++p;
		}
	}
	else
	{
		int imax = p - t1.size() + 1;
		for (int i = 0; i < imax; ++i)
		{
			t1 = std::string("0") + t1;
		}
		t1 = std::string(t1.c_str(), t1.size() - p) + "." + std::string(t1.c_str() + t1.size() - p, p);
	}

}

bool SetComma(std::string& t1, std::string& t2)
{
	int c1 = GetComma(t1);
	int c2 = GetComma(t2);

	if (c1 > c2)
	{
		if (c2 == 0) t2 += ".";
		for (int i = 0; i < c1 - c2; ++i)
		{
			t2 += "0";
		}
	}
	else if (c2 > c1)
	{
		if (c1 == 0) t1 += ".";
		for (int i = 0; i < c2 - c1; ++i)
		{
			t1 += "0";
		}
	}

	return c1 || c2;
}

void Reduce(std::string& expr)
{
	bool hasComma = GetComma(expr);
	int offset = 0;
	FillToSameSize(expr, expr);

	while (expr[offset] < '1')
	{
		bool isZero = expr[offset] == '0';
		bool sizeMoreThanOne = expr.size() > 1;
		bool nextIsComma = (hasComma && offset + 1 < expr.size() && (expr[offset + 1] == '.' || expr[offset + 1] == ','));
		if (isZero && sizeMoreThanOne && !nextIsComma)
			expr = std::string(expr.c_str(), offset) + (expr.c_str() + offset + 1);
		else if (expr[offset] == '-')
		{
			++offset;
			if (expr[offset] == '-')
			{
				expr = expr.c_str() + 2;
			}
		}
		else break;
	}

	while (hasComma)
	{
		if (expr[expr.size() - 1] == '0')
		{
			expr = std::string(expr.c_str(), expr.size() - 1);
		}
		else if (expr[expr.size() - 1] == '.' || expr[expr.size() - 1] == ',')
		{
			expr = std::string(expr.c_str(), expr.size() - 1);
			hasComma = false;
		}
		else break;
	}
}

int Compare(std::string a, std::string b)
{
	Reduce(a);
	Reduce(b);
	SetComma(a, b);
	FillToSameSize(a, b);

	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i] == b[i] || a[i] == '.' || a[i] == ',') continue;
		else if (a[i] > b[i]) return 1;
		else return -1;
	}
	return 0;
}

std::string Expression::Addition(std::string t1, std::string t2)
{
	Reduce(t1);
	Reduce(t2);

	std::string prefix = "";
	if (t1[0] == '-' && t2[0] == '-')
	{
		prefix = "-";
		t1 = t1.c_str() + 1;
		t2 = t2.c_str() + 1;
	}
	else if (t1[0] == '-')
	{
		return Subtract(t2, t1.c_str() + 1);
	}
	else if (t2[0] == '-')
	{
		return Subtract(t1, t2.c_str() + 1);
	}

	char hold = '0';
	SetComma(t1, t2);
	std::string result = std::string("0") + (t1.size() > t2.size() ? t1 : t2);
	int iMax = result.size() + 1;

	for (int i = 1; i < iMax; ++i)
	{
		int i1 = t1.size() - i;
		int i2 = t2.size() - i;
		int i3 = result.size() - i;

		if (i1 >= 0 && i2 >= 0)
		{
			if (t1[i1] == '.' || t1[i1] == ',') continue;
			result[i3] = CharAddition(t1[i1], t2[i2], hold);
		}
		else
		{
			result[i3] = CharAddition(result[i3], 48, hold);
		}
	}

	return prefix + result;
}

char CharSub(char c1, char c2, char& _hold)
{
	char i1 = c1 - 48;
	char i2 = c2 - 48;
	char h = _hold - 48;
	char res = (i1 - i2 - h);
	_hold = 0;

	while (res < 0)
	{
		res += 10;
		_hold += 1;
	}

	_hold += 48;
	return res + 48;
}

std::string Expression::Subtract(std::string t1, std::string t2)
{
	Reduce(t1);
	Reduce(t2);

	if (t1[0] == '-' && t2[0] == '-')
	{
		std::string tmp = t2.c_str() + 1;
		t2 = t1.c_str() + 1;
		t1 = tmp;
	}
	else if (t1[0] == '-')
	{
		return std::string("-") + Addition(t1.c_str() + 1, t2);
	}
	else if (t2[0] == '-')
	{
		return Addition(t1, t2.c_str() + 1);
	}

	std::string sign = "";

	if (Compare(t1, t2) == -1)
	{
		std::string tmp = t1;
		t1 = t2;
		t2 = tmp;
		sign = "-";
	}

	char hold = '0';
	SetComma(t1, t2);
	FillToSameSize(t1, t2);
	std::string result = t2;
	int iMax = result.size() + 1;

	for (int i = 1; i < iMax; ++i)
	{
		int i1 = t1.size() - i;
		int i2 = t2.size() - i;
		int i3 = result.size() - i;

		if (i1 >= 0 && i2 >= 0)
		{
			if (t1[i1] == '.' || t1[i1] == ',') continue;
			result[i3] = CharSub(t1[i1], t2[i2], hold);
		}
		else
		{
			result[i3] = CharSub(result[i3], 48, hold);
		}
	}
	return sign + result;
}

std::string CharMul(char c1, char c2)
{
	return std::to_string((c1 - 48) * (c2 - 48));
}

std::string GetSignAndSetAbsolute(std::string& t1, std::string& t2)
{
	std::string sign = "";
	if (t1[0] == '-' && t1[0] == t2[0])
	{
		t1 = t1.c_str() + 1;
		t2 = t2.c_str() + 1;
	}
	else if (t1[0] == '-')
	{
		if (t2[0] != '-')
		{
			t1 = t1.c_str() + 1;
			sign = "-";
		}
	}
	else if (t2[0] == '-')
	{
		if (t1[0] != '-')
		{
			t2 = t2.c_str() + 1;
			sign = "-";
		}
	}
	return sign;
}

std::string Expression::Multiply(std::string t1, std::string t2)
{
	std::string sign = "";
	std::string result = "0";

	Reduce(t1);
	Reduce(t2);

	sign = GetSignAndSetAbsolute(t1, t2);

	int c1 = GetComma(t1);
	int c2 = GetComma(t2);
	RemoveComma(t1);
	RemoveComma(t2);

	for (int i = 0; i < t1.size(); ++i)
	{
		int i1 = t1.size() - i - 1;
		for (int j = 0; j < t2.size(); ++j)
		{
			int i2 = t2.size() - j - 1;
			std::string tmp = CharMul(t1[i1], t2[i2]);
			for (int x = 0; x < i + j; ++x) tmp += "0";
			result = Addition(result, tmp);
		}
	}

	int c = c1 + c2;
	if (c) MulPowerOfTen(result, -c);
	return sign + result;
}

bool IsPowerOfTen(std::string a)
{
	int oneOccur = 0;
	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i] > '1') return false;
		else if (a[i] == '1') ++oneOccur;

		if (oneOccur > 1) return false;
	}

	return oneOccur == 1;
}

int GetPowerOfTen(std::string a)
{
	int c = GetComma(a);

	if (c) return -c;
	else return a.size() - 1;
}

std::string GetIntPart(std::string a)
{
	std::string res;
	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i] >= 48 && a[i] <= 57 || a[i] == 45)
			res += a[i];
		else
			break;
	}
	return res;
}

std::string Expression::EuclideanDiv(std::string t1, std::string t2)
{
	std::string result;
	std::string rest = "0";
	std::string divided;

	if (Compare(t2, "0") == 1 && Compare(t2, "1") == -1)
	{
		Expression rat = GetRationnal(t2);
		rat.Solve();
		rat.expr = std::string("(") + rat.secondTerm + "/" + rat.firstTerm + ")";
		rat.Solve();
		return Multiply(t1, rat);
	}

	int powTen = GetComma(t2) > 0 ? GetPowerOfTen(t2) : 0;
	int test = GetIntPart(t2).size();
	RemoveComma(t2);
	Reduce(t2);

	SetComma(t1, t2);
	FillToSameSize(t1, t2);
	int c1 = GetComma(t1);
	int c2 = GetComma(t2);
	int i = 0;
	int commaTrespassing = 0;
	int s2 = t1.size() - c1;
	while (commaTrespassing + powTen < 10)
	{
		int s1 = test - powTen + i;
		//int s1 = t2.size() - powTen + i;
		if (s1 > s2) ++commaTrespassing;
		result += "0";
		if (divided.size() == 0)
		{
			divided = std::string(t1.c_str(), s1);
			RemoveComma(divided);
			Reduce(divided);
		}

		std::string lastQ = "0";
		int j = 0;
		while (true)
		{
			std::string tmpQ = std::to_string(j);
			std::string tmpMul = Multiply(tmpQ, t2);
			std::string lastMul = Multiply(lastQ, t2);
			Reduce(tmpMul);
			Reduce(lastMul);
			if (Compare(divided, tmpMul) == 0)
			{
				lastQ = tmpQ;
				break;
			}
			else if (Compare(divided, lastMul) == 1 && Compare(divided, tmpMul) == -1)
			{
				break;
			}
			else
			{
				lastQ = tmpQ;
			}
			++j;
		}

		result = Addition(result, lastQ);

		divided = Subtract(divided, Multiply(lastQ, t2));

		if (s1 >= t1.size())
		{
			if (Compare(divided, "0") == 0) break;
			divided += "0";
		}
		else
		{
			divided += t1[s1];
		}

		RemoveComma(divided);
		Reduce(divided);
		++i;
	}

	MulPowerOfTen(result, -commaTrespassing - powTen);

	return result;
}

std::string Expression::Divide(std::string t1, std::string t2)
{
	Reduce(t1);
	Reduce(t2);

	std::string sign = GetSignAndSetAbsolute(t1, t2);
	std::string result;

	if (t2 == "0") return "inf";
	if (t1 == "0") return "0";

	if (IsPowerOfTen(t2))
	{
		int c = GetPowerOfTen(t2);
		if (c != 0)
		{
			MulPowerOfTen(t1, -c);
			result = t1;
		}
		result = t1;
	}
	else
	{
		result = EuclideanDiv(t1, t2);
	}

	return sign + result;
}

void Expression::ReduceFraction(std::string& t1, std::string& t2)
{
	while (true)
	{
		std::string num = t1;
		std::string den = t2;
		std::string i = "1";
		char l1 = num[num.size() - 1] - 48;
		char l2 = den[den.size() - 1] - 48;

		if (l1 % 2 == 0 && l2 % 2 == 0)
		{
			i = "2";
		}
		else if ((l1 == 5 || l1 == 0) && (l2 == 5 || l2 == 0))
		{
			i = "5";
		}
		else if ((l1 == 3 || l1 == 6 || l1 == 9) && (l2 == 3 || l2 == 6 || l2 == 9))
		{
			i = "3";
		}

		for (int x = 0; x < 10; ++x)
		{
			std::string tmpNum = Divide(t1, i);
			std::string tmpDen = Divide(t2, i);
			Reduce(tmpNum);
			Reduce(tmpDen);
			if (GetComma(tmpNum) == 0 && GetComma(tmpDen) == 0)
			{
				num = tmpNum;
				den = tmpDen;
				i = Addition(i, "1");
				Reduce(i);
			}
			else
			{
				break;
			}
		}

		if (Compare(num, t1) != 0)
		{
			t1 = num;
			t2 = den;
		}
		else
		{
			break;
		}
	}
}

Expression Expression::GetRationnal(std::string t1)
{
	int c = GetComma(t1);
	if (c == 0) return t1;

	std::string denominator = "1";

	MulPowerOfTen(t1, c);
	MulPowerOfTen(denominator, c);
	RemoveComma(t1);
	ReduceFraction(t1, denominator);

	return t1 + "/" + denominator;
}

std::string Expression::NaturalPower(std::string t1, std::string t2)
{
	if (t2.size() > 2) Cout << "<=== warning heavy power calculation : Pow(" << t1 << ", " << t2 << ") ===>" << std::endl;
	std::string res = t1;

	if (Compare(t2, "0") == 0) return "1";

	while (Compare(t2, "1") != 0)
	{
		res = Multiply(res, t1);
		t2 = Subtract(t2, "1");
	}
	return res;
}

std::string Expression::Root(std::string t1, std::string t2)
{
	if (t2.size() > 1) Cout << "<=== warning heavy root calculation : Root(" << t1 << ", " << t2 << ") ===>" << std::endl;
	std::string root = std::string(t1.c_str(), t1.size() - GetComma(t1));
	for (int i = 0; i < root.size(); ++i) root[i] = '0';
	int offset = 0;
	int initSize = root.size();

	while (offset - initSize < 15)
	{
		if (offset == initSize)
		{
			root += '.';
			++offset;
		}

		if (offset >= initSize)
		{
			root += "0";
		}

		for (int i = 0; i < 10; ++i)
		{
			std::string tmpRoot = root;

			tmpRoot[offset] = i + 48;
			int comparison = Compare(Power(tmpRoot, t2), t1);
			if (comparison == 0)
			{
				root = tmpRoot;
				Reduce(root);
				return root;
			}
			else if (comparison == -1) root = tmpRoot;
			else break;
		}
		if (Compare(t1, Power(root, t2)) == 0) break;
		++offset;
	}
	Reduce(root);
	return root;
}

std::string Expression::Power(std::string t1, std::string t2)
{
	Reduce(t1);
	Reduce(t2);

	if (Compare(t1, "0") == 0) return t1;

	if (t2[0] == '-')
	{
		return Divide("1", Power(t1, t2.c_str() + 1));
	}

	if (Compare(t2, "0") == 0) return "1";
	if (Compare(t2, "1") == 0) return t1;

	Expression rat = GetRationnal(t2);

	if (Compare(rat, "0") == 0) return "1";
	else if (rat.op == Num)
	{
		return NaturalPower(t1, rat.expr);
	}
	else
	{
		std::string r = Root(t1, rat.secondTerm);
		return NaturalPower(r, rat.firstTerm);
	}
}

void Expression::RemoveSpaces()
{
	int iMax = expr.size() - 1;
	for (int i = iMax; i >= 0; --i)
	{
		if (expr[i] == ' ')
		{
			expr = std::string(expr.c_str(), i) + std::string(expr.c_str() + i + 1);
		}
	}
}

void Expression::RemoveUselessBrackets()
{
	bool areBracketsBracketting = true;

	while (areBracketsBracketting)
	{
		int firstChar = -1;
		int lastChar = -1;
		int brackets = 0;
		int lastBrackets = 0;

		for (int i = 0; i < expr.size(); ++i)
		{
			if (expr[i] >= 40/* && expr[i] <= 57*/)
			{
				if (firstChar == -1)
				{
					firstChar = i;
				}
				else
				{
					lastChar = i;
				}


				if (expr[i] == '(')
				{
					lastBrackets = brackets;
					++brackets;
				}
				else if (expr[i] == ')')
				{
					lastBrackets = brackets;
					--brackets;
				}
				if (brackets < 0) return;
				if (brackets == 0 && expr[i] >= 42/* && expr[i] <= 57*/)
				{
					areBracketsBracketting = false;
					return;
				}
			}
		}

		if (firstChar < 0 || lastChar < 0) return;
		if (expr[firstChar] == '(' && expr[lastChar] == ')')
		{
			expr = std::string(expr.c_str() + firstChar + 1, lastChar - firstChar - 1);
		}
	}
}

bool Expression::CheckIsNumber()
{
	return CheckIsNumber(expr);
}

bool Expression::CheckIsNumber(std::string a)
{
	if (a.size() == 0) return false;
	for (int i = 0; i < a.size(); ++i)
	{
		if ((a[i] < 48 || a[i] > 57) && !(a[i] == '.' || a[i] == ',' || (i == 0 && a[i] == '-' && a.size() > 1)))
		{
			return false;
		}
	}
	return true;
}

bool IsOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

std::string Expression::GetOperation()
{
	switch (op)
	{
	case Add:
		return "+";
	case Sub:
		return "-";
	case Mul:
		return "*";
	case Div:
		return "/";
	case Pow:
		return "^";
	default:
		return "Invalid or just a number";
	}
}

void Expression::ReduceExpr()
{
	RemoveSpaces();
	RemoveUselessBrackets();

	if (expr.size() > 4)
	{
		while (expr.substr(0, 4) == "-(-(" && expr.substr(expr.size() - 2, 2) == "))")
		{
			expr = expr.substr(3, expr.size() - 4);
			RemoveUselessBrackets();
		}
	}
}

void Expression::Solve(bool _log)
{
	int brackets = 0;
	int id = -1;
	int priority = 10;
	char operation = ' ';
	log = _log;

	ReduceExpr();

	if (CheckIsNumber())
	{
		Reduce(expr);
		res = expr;
		solved = true;
		op = Num;
		return;
	}

	for (int i = 0; i < expr.size(); ++i)
	{
		if ((expr[i] < 40 || expr[i] > 57) && expr[i] != 94)
		{
			res = "unexpected token";
			return;
		}

		int tmpPrio = 10;
		Operation tmpOp = Invalid;
		char tmpOperation = ' ';

		if (expr[i] == '(')
		{
			++brackets;
		}
		else if (expr[i] == ')')
		{
			--brackets;
		}

		if (brackets > 0)
		{
			continue;
		}
		else if (brackets < 0)
		{
			res = "too much closing brackets";
			return;
		}
		else if (brackets == 0)
		{
			if (expr[i] == '*')
			{
				tmpPrio = 2;
				tmpOp = Mul;
				tmpOperation = expr[i];
			}
			else if (expr[i] == '+')
			{
				tmpPrio = 1;
				tmpOp = Add;
				tmpOperation = expr[i];
			}
			else if (expr[i] == '-' && !(i - 1 < 0 || IsOperator(expr[i - 1])))
			{
				tmpPrio = 1;
				tmpOp = Sub;
				tmpOperation = expr[i];
			}
			else if (expr[i] == '/')
			{
				tmpPrio = 3;
				tmpOp = Div;
				tmpOperation = expr[i];
			}
			else if (expr[i] == '^')
			{
				tmpPrio = 4;
				tmpOp = Pow;
				tmpOperation = expr[i];
			}
		}

		if (tmpPrio < priority)
		{
			priority = tmpPrio;
			op = tmpOp;
			operation = tmpOperation;
			id = i;
		}
	}

	if (id == -1)
	{
		if (expr[0] == '-')
		{
			res = std::string("-") + std::string(Expression(expr.c_str() + 1));
			Reduce(res);
			solved = true;
			return;
		}
		res = "invalid operation";
		return;
	}

	if (brackets > 0)
	{
		res = "invalid open brackets";
		return;
	}
	else
	{
		OperationFct operationFct = &Expression::Addition;

		switch (op)
		{
		case Add:
			operationFct = &Expression::Addition;
			break;
		case Sub:
			operationFct = &Expression::Subtract;
			break;
		case Mul:
			operationFct = &Expression::Multiply;
			break;
		case Div:
			operationFct = &Expression::Divide;
			break;
		case Pow:
			operationFct = &Expression::Power;
			break;
		case Invalid:
			res = "Invalid operation";
			return;
		}

		Expression tmpExpr1 = Expression(std::string(expr.c_str(), id));
		Expression tmpExpr2 = Expression(std::string(expr.c_str() + id + 1, expr.size() - (id + 1)));

		std::string opStr = std::string(tmpExpr1) + " " + operation + " " + std::string(tmpExpr2);

		if (op == Mul)
		{
			Cout << "<=== " << expr << " : checking for optimisation ===>" << std::endl;
			tmpExpr1.Solve();
			tmpExpr2.Solve();

			std::string tmpOp1 = tmpExpr1.GetOperation();
			std::string tmpOp2 = tmpExpr2.GetOperation();
			std::string tmpExpr =
				(tmpExpr1.op <= 3 ? tmpExpr1.firstTerm + tmpOp1 + tmpExpr1.secondTerm : std::string(tmpExpr1))
				+ operation
				+ (tmpExpr2.op <= 3 ? tmpExpr2.firstTerm + tmpOp2 + tmpExpr2.secondTerm : std::string(tmpExpr2));

			if (tmpExpr1.op == Div && tmpExpr2.op <= Div)
			{
				Cout << "<=== " << tmpExpr << " : multiplying left division's first term with right expression before dividing ===>" << std::endl;
				expr = std::string(Expression(tmpExpr1.firstTerm + "*" + std::string(tmpExpr2))) + "/" + tmpExpr1.secondTerm;
				Solve();
				return;
			}
			else if (tmpExpr2.op == Div && tmpExpr1.op <= Div)
			{
				Cout << "<=== " << tmpExpr << " : multiplying right division's first term with left expression before dividing ===>" << std::endl;
				expr = std::string(Expression(tmpExpr2.firstTerm + "*" + std::string(tmpExpr1))) + "/" + tmpExpr2.secondTerm;
				Solve();
				return;
			}
			else if (tmpExpr2.op == Div && tmpExpr1.op == Div)
			{
				Cout << "<=== " << tmpExpr << " : multiplying both division's first terms and second terms together before dividing firsts product by seconds product ===>" << std::endl;
				expr = std::string(Expression(tmpExpr1.firstTerm + "*" + tmpExpr2.firstTerm)) + "/" + std::string(Expression(tmpExpr1.secondTerm + "*" + tmpExpr2.secondTerm));
				Solve();
				return;
			}
			Cout << "<=== " << tmpExpr << " : continuing normally ===>" << std::endl;
		}
		else if (op == Pow)
		{
			Cout << "<=== " << expr << " : checking for optimisation ===>" << std::endl;
			tmpExpr1.Solve();
			tmpExpr2.Solve();
			std::string tmpExpr = expr;
			if (std::string(tmpExpr2)[0] == '-' && tmpExpr2.secondTerm[0] != '-')
			{
				expr = std::string("1/") + "(" + std::string(tmpExpr1) + "^(" + (tmpExpr2.expr.c_str() + 1) + "))";
				Cout << "<=== " << expr << " : negative power (y) is 1 over x^(-y) ===>" << std::endl;
				Solve();
				return;
			}
			else if (tmpExpr2.op == Div && tmpExpr2.secondTerm[0] != '-' && GetComma(tmpExpr2.secondTerm) == 0)
			{
				tmpExpr = std::string("Pow(Root(") + std::string(tmpExpr1) + "," + tmpExpr2.secondTerm + "), " + tmpExpr2.firstTerm + ")";
				Cout << "<=== " << tmpExpr << " : if power is a fraction, changing to pow(root(a, b2), b1) ===>" << std::endl;
				expr = std::string("(") + Root(tmpExpr1, tmpExpr2.secondTerm) + ")^(" + tmpExpr2.firstTerm + ")";
				Solve();
				return;
			}
			/*else if (Compare(tmpExpr2, "0") == 1 && Compare(tmpExpr2, "1") == -1)
			{
				tmpExpr = tmpExpr1.expr + "^(" + GetRationnal(tmpExpr2).expr + ")";
				Cout << "<=== " << tmpExpr << " : change fraction of one to rationnal instead of decimal ===>" << std::endl;
				expr = tmpExpr;
				Solve();
				return;
			}*/
			else
			{
				Cout << "<=== " << tmpExpr << " : continuing normally ===>" << std::endl;
			}
		}

		firstTerm = tmpExpr1;
		secondTerm = tmpExpr2;

		if (CheckIsNumber(firstTerm) && CheckIsNumber(secondTerm))
		{
			res = (this->*operationFct)(firstTerm, secondTerm);
			Reduce(res);
		}
		else
		{
			res = "invalid expression";
		}

		Cout << opStr << " = " << res << std::endl;
		solved = true;
	}
}

Expression::operator std::string()
{
	if (!solved) Solve();
	return res;
}
