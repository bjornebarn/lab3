/*
 * expression_tree.cc    2011-06-28
 */
#include "variable_table.h"
#include "expression_tree.h"
#include <iostream>
#include <sstream>
#include <math.h>
// Inkludera för allt som kommer att användas i denna fil!
using namespace std;

// Separata definitioner för för Expression_Tree-klasserna definieras här.

Binary_Operator::Binary_Operator(Expression_Tree* left, Expression_Tree* right)
{
    lhs = left;
    rhs = right;
}

Assign::Assign(Expression_Tree* left, Expression_Tree* right, Variable_Table* temp_table)
    : Binary_Operator(left,right)
{
    var_table = temp_table;
}

Plus::Plus(Expression_Tree* left, Expression_Tree* right)
    : Binary_Operator(left,right) {}

Minus::Minus(Expression_Tree* left, Expression_Tree* right)
    : Binary_Operator(left,right) {}

Times::Times(Expression_Tree* left, Expression_Tree* right)
    : Binary_Operator(left,right) {}

Divide::Divide(Expression_Tree* left, Expression_Tree* right)
    : Binary_Operator(left,right) {}

Power::Power(Expression_Tree* left, Expression_Tree* right)
    : Binary_Operator(left,right) {}

Integer::Integer(int x) { i = x; }

Real::Real(double y) { d = y; }

Variable::Variable(string str, Variable_Table* temp_table) { var = str; var_table = temp_table; }

Binary_Operator::~Binary_Operator()
{
    delete lhs; delete rhs;
}

double Assign::evaluate()
{
    double temp = rhs->evaluate();
    var_table->insert(lhs->str(), temp);
    return temp;
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

double Integer::evaluate() { return (double) i; }

double Real::evaluate() { return d; }

double Variable::evaluate() { return var_table->get_value(var); }

string Assign::str() { return "="; }
string Plus::str() { return "+"; }
string Minus::str() { return "-"; }
string Times::str() { return "*"; }
string Divide::str() { return "/"; }
string Power::str() { return "^"; }
string Integer::str()
{
    stringstream out;
    out << i;
    return out.str();
}
string Real::str()
{
    stringstream out;
    out << d;
    return out.str();
}
string Variable::str()
{
    return var;
}


string Binary_Operator::get_postfix()
{
    return lhs->get_postfix().append(" "+rhs->get_postfix().append(" "+str()));
}

string Operand::get_postfix() { return str(); }

string Binary_Operator::get_infix()
{
    string temp_lhs = "";
    string temp_rhs = "";
    if (str() == "+")
        return lhs->get_infix().append(" + "+rhs->get_infix());

    else if (str() == "-" or str() == "*")
    {
        if (lhs->str() == "+" or lhs->str() == "-")
            temp_lhs = "("+lhs->get_infix()+")";
        else
            temp_lhs = lhs->get_infix();
        if (rhs->str() == "+" or rhs->str() == "-")
            temp_rhs = "("+rhs->get_infix()+")";
        else
            temp_rhs = rhs->get_infix();
        return temp_lhs+" "+str()+" "+temp_rhs;
    }

    else if (str() == "/")
    {
        if (lhs->str() == "-" or lhs->str() == "+" or lhs->str() == "/")
            temp_lhs = "("+lhs->get_infix()+")";
        else
            temp_lhs = lhs->get_infix();
        if (rhs->str() == "-" or rhs->str() == "+" or rhs->str() == "/" or rhs->str() == "*")
            temp_rhs = "("+rhs->get_infix()+")";
        else
            temp_rhs = rhs->get_infix();
        return temp_lhs+" / "+temp_rhs;
    }
    else if (str() == "^")
    {
        if (lhs->str() == "-" or lhs->str() == "+" or lhs->str() == "/" or lhs->str() == "*")
            temp_lhs = "("+lhs->get_infix()+")";
        else
            temp_lhs = lhs->get_infix();
        if (rhs->str() == "-" or rhs->str() == "+" or rhs->str() == "/" or rhs->str() == "*")
            temp_rhs = "("+rhs->get_infix()+")";
        else
            temp_rhs = rhs->get_infix();
        return temp_lhs+" ^ "+temp_rhs;
    }
    else if (str() == "=")
        return lhs->get_infix()+" = "+rhs->get_infix();
    return "";
}

string Operand::get_infix() { return str(); }


void Binary_Operator::print(ostream& os, string indent)
{
    rhs->print(os, indent + "  ");
    os << indent << " /\n";
    os << indent << str() << "\n";
    os << indent << " \\\n";
    lhs->print(os, indent + "  ");
}

void Integer::print(ostream& os, string indent)
{
    os << indent << i << "\n";
}

void Real::print(ostream& os, string indent)
{
    os << indent << d << "\n";
}

void Variable::print(ostream& os, string indent)
{
    os << indent << var << "\n";
}
Expression_Tree* Assign::clone()
{
    Expression_Tree* clone = new Assign(lhs->clone(), rhs->clone(), var_table);
    return clone;
}

Expression_Tree* Plus::clone()
{
    Expression_Tree* clone = new Plus(lhs->clone(), rhs->clone());
    return clone;
}

Expression_Tree* Minus::clone()
{
    Expression_Tree* clone = new Minus(lhs->clone(), rhs->clone());
    return clone;
}

Expression_Tree* Times::clone()
{
    Expression_Tree* clone = new Times(lhs->clone(), rhs->clone());
    return clone;
}

Expression_Tree* Divide::clone()
{
    Expression_Tree* clone = new Divide(lhs->clone(), rhs->clone());
    return clone;
}

Expression_Tree* Power::clone()
{
    Expression_Tree* clone = new Power(lhs->clone(), rhs->clone());
    return clone;
}

Expression_Tree* Integer::clone()
{
    Expression_Tree* clone = new Integer(i);
    return clone;
}

Expression_Tree* Real::clone()
{
    Expression_Tree* clone = new Real(d);
    return clone;
}

Expression_Tree* Variable::clone()
{
    Expression_Tree* clone = new Variable(var, var_table);
    return clone;
}


void Variable::set_value(double value) { var_table->set_value(var, value); }

double Variable::get_value() { return var_table->get_value(var); }
