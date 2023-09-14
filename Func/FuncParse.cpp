#include "FuncParse.h"
#include <cmath>
#include <unordered_set>

namespace yxqin
{
constexpr double pi=3.14159265;
constexpr double e=2.718281828;
const static std::unordered_set<char> xKeySet={'x','y','z'};//可能为未知数的集合
const static std::unordered_set<char> numSet={'0','1','2','3','4','5','6','7','8','9','p','e'};
static const std::type_info& doubleId= typeid(double);
//判断是否为 操作符 + -* / ^ 包含 sin cos ,不会移动i
bool isOpt(const std::string& expr,int i)
{
	switch (expr[i])
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
		case 's':
		case 'c':
		case 't':
		case 'l':
		case '(':
		case ')':
			return true;
		default:
			return false;
	}
}
//判断是否为 数值 包含 pi e   不会移动i
bool isNumber(const std::string& expr,int i)
{

	if(expr[i]=='-')
		return (++i)<expr.length()&& numSet.find(expr[i])!=numSet.end();
	else
		return numSet.find(expr[i])!=numSet.end()||xKeySet.find(expr[i])!=xKeySet.end();
}

//优先级 '(' < 加减 < 乘除 < 函数 < 指数
int getPriority(char c)
{
	switch (c)
	{
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		case 's':
		case 'c':
		case 't':
		case 'g':
		case 'n':
			return 3;
		case '^':
			return 4;
		default:
			return 0;
	}
}
//得到number 会移动i
double getNumber(const std::string& expr,int &i)
{
	auto len=expr.length();
	double sign=1;//符号
	if(i<len&&expr[i]=='-'){
		sign=-1;
		++i;
	}
	if(i<len&&expr[i]=='p'){
		i+=2;
		return pi*sign;
	}
	if(i<len&&expr[i]=='e'){
		++i;
		return pi*e;
	}
	//处理实数
	double ret=0;
	while (i < len && expr[i]>='0'&&expr[i]<='9')
	{
		ret = ret * 10 + expr[i] - '0';
		i++;
	}
	double point=1;//小数点
	if(i<len&&expr[i]=='.')
	{
		point=0;
		++i;
		while (i < len && expr[i]>='0'&&expr[i]<='9')
		{
			ret = ret * 10 + expr[i] - '0';
			point+=10;
			i++;
		}
	}
	return ret*sign/point;
}
//获取操作符 会移动i
char getOpt(const std::string& expr,int &i)
{
	auto len=expr.length();
	if(i<len)
		switch (expr[i])
		{
			case '+':
			case '-':
			case '*':
			case '/':
			case '^':
			case '(':
			case ')':
				++i;
				return expr[i-1];
			case 's':
			case 'c':
			case 't':
				i+=3;
				return expr[i-3];
			case 'l':
				i+=2;
				if(i-1<len)
					return expr[i-1];

		}
	return '?';
}
//判断是否为函数 如sin cas tan 等
bool isFuncOpt(char c)
{
	switch (c)
	{
		case 's':
		case 'c':
		case 't':
		case 'n':
		case 'g':
			return true;
		default:
			return false;
	}
}

//检查后缀表达式 正确性 ，这里主要检查表达式的中的操作数和操作符号是否匹配
bool checkPost(const std::vector<std::any>& post)
{

	int n=0;//统计数的个数
	for(auto& item:post)
	{
		char c='?';
		if(item.type()==doubleId)//数 n++
			++n;
		else
		{
			c=std::any_cast<char>(item);
			if(xKeySet.find(c)!=xKeySet.end())//数 n++
				++n;
			else
			{
				//符号 会取数 需要额外判断
				if(isFuncOpt(c))//sin cos 需要 有一个操作数
				{
					if(n<1)
						return false;
				}else if(n>=2)//其他操作需要两个操作数
					--n;
				else//操作数不够 错误
					return false;
			}
		}

	}
	return true;
}

bool getPostfix(std::string  expr,std::vector<std::any>& ret)
{
	ret.clear();
	std::vector<char> stack;//符号栈
	int i=0;
	auto len=expr.length();
	try
	{
		while (i<len)
		{
			if(isNumber(expr,i))
			{
				if(expr[i]=='x')
				{
					++i;
					ret.push_back('x');
				}
				else
					ret.push_back(getNumber(expr,i));
			}else if(isOpt(expr,i))//如果是符号
			{
				int ii=i;
				char now= getOpt(expr,i);
				if(now ==')')//如果遇到右括号，则符号栈一直pop，pop的符号push到总栈中，直到遇到左括号，消除括号
				{
					while(!stack.empty()&&stack.back()!='(')
					{

						ret.push_back(stack.back());
						stack.pop_back();
					}
					if(!stack.empty())
						stack.pop_back();
				}else if(stack.empty()||now=='('||stack.back()=='(')// 如果当前符号栈为空或遇到左括号或栈顶是左括号，直接入符号栈
					stack.push_back(now);
				else// 如果符号栈不空，就比较当前运算符和栈顶运算符的优先级
				{
					if(getPriority(now)>getPriority(stack.back())||isFuncOpt(now)&&isFuncOpt(stack.back()))  // 当前运算符小于或等于栈顶运算符的优先级，则pop出栈顶符号，push到总栈中，回溯 i
						stack.push_back(now);
					else
					{
						ret.push_back(stack.back());
						stack.pop_back();
						i=ii;
					}
				}
			}else//其他符号 存在错误
			{
				return false;
			}

		}
		//栈不为空 全部push dao ret
		while(!stack.empty())
		{
			ret.push_back(stack.back());
			stack.pop_back();
		}
	}catch (...)
	{
		return false;
	}
	return checkPost(ret);
}

bool getResult(const std::vector<std::any>& post ,double& ret, char k,double keyVal)
{
	std::vector<double> stack;

	ret=0;
	try
	{
		for(auto& item:post)
		{
			//处理数
			if(item.type()==doubleId)
				stack.push_back(std::any_cast<double>(item));
			else
			{
				char c=std::any_cast<char>(item);
				if(c==k)
					stack.push_back(keyVal);
				else//计算
				{
					double tmp=0;
					//函数 一个数进行计算
					if(isFuncOpt(c))
					{
						if(stack.size()<1)
							return false;
						double num=stack.back();
						stack.pop_back();
						switch (c)
						{
							case 's':
								tmp=sin(num);
								break;
							case 'c':
								tmp=cos(num);
								break;
							case 't':
								tmp= tan(num);
								break;
							case 'g':
								tmp=log10(num);
								break;
							case 'n':
								tmp=log(num);
								break;
							default:
								tmp=0;
								break;
						}
					}
					else // 双目运算符运算
					{
						if(stack.size()<2)
							return false;
						double num1=stack.back();
						stack.pop_back();
						double num2=stack.back();
						stack.pop_back();
						switch (c)
						{
							case '+':
								tmp=num2+num1;
								break;
							case '-':
								tmp=num2-num1;
								break;
							case '*':
								tmp=num2*num1;
								break;
							case '/':
								tmp=num2/num1;
								break;
							case '^':
								tmp= pow(num2,num1);
								break;
							default:
								tmp=0;
								break;
						}
					}
					stack.push_back(tmp);
				}
			}
		}
	}catch (...)
	{
		return false;
	}
	if(stack.size()!=1)
		return false;
	ret=stack.back();
	return true;
}
}//namespace yxqin