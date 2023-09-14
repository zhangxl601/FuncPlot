#ifndef FOOPLOT_EXPRESSION_H
#define FOOPLOT_EXPRESSION_H
/*
 *
 * 处理所有表达式
 *
 * */
#include <iostream>
#include <string>
#include <vector>
#include <any>

/**
 *@brief  函数表达式解析相关函数
 *
 * 支持解析cos sin tan ^ 以及基本四则运算 的含未知数x,y,z表达式
 */
namespace yxqin
{
/**
 * @brief 取得算式的后缀表达式
 *
 * 将中缀表达式转为后缀表达式，并且将数值也转换为double
 * @param expr 中缀表达式
 * @param &ret 得到的后缀表达式
 * @return
 * 简单检查表达式中的错误
 */
bool getPostfix(std::string  expr,std::vector<std::any>& ret);

/**
 * @brief 后缀表达式求值
 *
 * 目前仅支持含有一个未知数的表达式
 *
 * @param &post 后缀表达式
 * @param &ret  返回结果
 * @param k 式子中的未知数
 * @param keyVal 未知数的值
 * @return
 * 如果计算异常返回错误
 */
bool getResult(const std::vector<std::any>& post ,double& ret, char k,double keyVal);//根据后缀表达式计算 结果ret 返回 是否计算成功

}


#endif //FOOPLOT_EXPRESSION_H
