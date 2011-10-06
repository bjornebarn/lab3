/*
 * expression_tree.cc    2011-06-28
 */
#ifndef EXPRESSION_TREE
#define EXPRESSION_TREE
#include "expression_tree.h"
#include <iostream>
#include <sstream>
#include <math.h>
// Inkludera för allt som kommer att användas i denna fil!
using namespace std;

// Separata definitioner för för Expression_Tree-klasserna definieras här.


Assign::Assign(Expression_Tree* var, Expression_Tree* right)
{
    lhs = var;
    rhs = right;
}

Plus::Plus(Expression_Tree* left, Expression_Tree* right)
{
    lhs = left;
    rhs = right;
}

Minus::Minus(Expression_Tree* left, Expression_Tree* right)
{
    lhs = left;
    rhs = right;
}

Times::Times(Expression_Tree* left, Expression_Tree* right)
{
    lhs = left;
    rhs = right;
}

Divide::Divide(Expression_Tree* left, Expression_Tree* right)
{
    lhs = left;
    rhs = right;
}

Power::Power(Expression_Tree* left, Expression_Tree* right)
{
    lhs = left;
    rhs = right;
}

Integer::Integer(int x) { num_var = x; }

Real::Real(double y) { num_var = y; }

Variable::Variable(string str) { num_var = str; }

double Assign::evaluate()
{
    double temp = lhs->evaluate(); //Måste sätta temp till en variabel!!!
}

double Plus::evaluate()
{
    return lhs->evaluate() + rhs->evaluate();
}

double Minus::evaluate()
{
    return lhs->evaluate() - rhs->evaluate();
}

double Times::evaluate()
{
    return lhs->evaluate() * rhs->evaluate();
}

double Divide::evaluate()
{
    return lhs->evaluate() / rhs->evaluate();
}

double Power::evaluate()
{
    return pow(lhs->evaluate(), rhs->evaluate());
}

double Integer::evaluate() { return (double) num_var; }

double Real::evaluate() { return num_var; }

//double Variable::evaluate() { return var_list(var); }
 
string Assign::str() { return "="; }
string Plus::str() { return "+"; }
string Minus::str() { return "-"; }
string Times::str() { return "*"; }
string Divide::str() { return "/"; }
string Power::str() { return "^"; }
string Integer::str() 
{ 
    stringstream out;
    out << num_var;
    return out.str(); 
}       
string Real::str()
{
    stringstream out;
    out << num_var;
    return out.str();
}
string Variable::str()
{
    return num_var;
}


string Binary_Operator::get_postfix()
{
    return lhs->get_postfix().append(rhs->get_postfix().append(str()));
}

string Operand::get_postfix() { return str(); }


void Binary_Operator::print(ostream& os)
{
    rhs->print(os << "  ");
    cout << " /\n" << str() << "\n \\\n";
    lhs->print(os << "  ");
}

void Operand::print(ostream& os)
{
    cout << " /\n" << num_var << "\n \\\n";
}    

Expression_Tree* Binary_Operator::clone()
{
    Expression_Tree* clone = new this(lhs->clone(), rhs->clone());
    return clone;
}

Expression_Tree* Operand::clone()
{
    Expression_Tree* clone = new this(num_var);
    return clone;
}

//void Variable::set_value(double value) { set_list(num_var, value); }

//double Variable::get_value() { blalbalba ; }
//
#endif
