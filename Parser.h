#include <string>  //std::wstring
#include <vector>  //std::vector
#include <stack>   //std::stack
#include <cctype>  //isdigit()
#include <cstdlib> //stod()
#include <cmath>   //std::pow
#include <map>     //std::map
#include <iostream>


//definition of operation function
double plus(double lhs, double rhs)
{
	return lhs + rhs;
}

double subtract(double lhs, double rhs)
{
	return lhs - rhs;
}

double multiple(double lhs, double rhs)
{
	return lhs*rhs;
}

double divide(double lhs, double rhs)
{
	return lhs / rhs;
}

double parallel(double lhs, double rhs)
{
	return (1 / ((1 / lhs) + (1 / rhs)));
}

double powoper(double lhs, double rhs)
{
	return std::pow(lhs, rhs);
}



std::map<char, double(*)(double, double)> oper_map
{
	std::make_pair('+', plus),
	std::make_pair('-', subtract),
	std::make_pair('*', multiple),
	std::make_pair('/', divide),
	std::make_pair('|', parallel),
	std::make_pair('^', powoper)
};

int comparePiority(const char lhs, const char rhs); // lhs>rhs returns 1, lhs=rhs returns 0, lhs<rhs return -1
int getPiority(const char oper);

std::wstring infixtorpn(const std::wstring &infix)
{
	std::stack<char> op_stk;
	std::wstring res;
	for(auto iter=infix.begin();
			iter!=infix.end()||!op_stk.empty();
			/*Empty*/)
	{
		if (iter==infix.end()&&!op_stk.empty())
		{
			while(!op_stk.empty())
			{
				char c = op_stk.top();
				res.append(1,c);
				res.append(L" ");
				op_stk.pop();
			}		
		}
		else if(std::isdigit(*iter))
		{
			auto beg=iter;
			while (iter != infix.end()&&(std::isdigit(*iter) || *iter == '.'))
			{
				++iter;
			}
			res.append(infix,beg-infix.begin(),iter-beg);
			res.append(L" "); //for spacing
		}
		else if(*iter=='(')
		{
			op_stk.push(*iter);
			++iter;
		}
		else if(*iter==')')
		{
			while(op_stk.top()!='(')
			{
				const char c = op_stk.top();
				res.append(1,c);
				res.append(L" ");
				op_stk.pop();
			}
			op_stk.pop(); //pop '('
			++iter;
		}
		else //for other operator
		{
			if (!op_stk.empty())
			{
				while (!op_stk.empty()&&comparePiority(op_stk.top(), *iter) > -1)
				{
					const char c = op_stk.top();
					res.append(1, c);
					res.append(L" ");
					op_stk.pop();
				}
			}
			op_stk.push(*iter);
			++iter;
		}
	}
	return res;
}

int comparePiority(const char lhs, const char rhs)
{
	if(getPiority(lhs)>getPiority(rhs))
		return 1;
	else if(getPiority(lhs)==getPiority(rhs))
		return 0;
	else
		return -1;
}

int getPiority(const char oper)
{
	switch(oper)
	{
		case '(':
			return 1;
		case '+':
		case '-':
			return 2;
		case '|':
		case '*':
		case '/':
			return 3;
		case '^':
			return 4;
	}
}

double evalrpn(const std::wstring &rpn)
{
	std::stack<double> stk;
	for (auto iter = rpn.begin();
		iter != rpn.end();
		/*Empty*/)
	{ 
		if (std::isdigit(*iter))
		{
			auto beg = iter;			
			while (*iter != ' ') //make iter reaching spacing char
			{
				++iter;
			}
			stk.push(std::stod(rpn.substr(beg - rpn.begin())));
			++iter; //points to the next
		}
		else //the iter points to an operator
		{
			if (stk.size() > 1)
			{
				double lhs = stk.top();
				stk.pop();
				double rhs = stk.top();
				stk.pop();
				stk.push(oper_map.find(*iter)->second(lhs, rhs));
			}
			else //something goes wrong
			{
				throw std::runtime_error("The expression is illegal");
			}
			++iter;//to skip spacing char
			++iter; 
		}
	}
	return stk.top(); 
}