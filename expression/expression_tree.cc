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

class expression_tree_error : public logic_error
{
    public:
    explicit expression_tree_error(const string& what_arg) throw()
        : logic_error(what_arg) {}
};
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
        double temp_num = rhs->evaluate();
        if (temp_num == 0)
            throw expression_tree_error("Division by zero");
        return lhs->evaluate() / temp_num;
}

double Power::evaluate()
{
        double l_temp = lhs->evaluate();
        double r_temp = rhs->evaluate();
        if (l_temp < 0 and r_temp < 1 and r_temp > -1)
            throw expression_tree_error("Complex numbers not implemented");
    return pow(l_temp, r_temp);
}

double Integer::evaluate()
{
    return (double) i;
}

double Real::evaluate() 
{
    return d;
}

double Variable::evaluate()
{
    return var_table->get_value(var);
}

string Assign::str() const { return "="; }
string Plus::str() const { return "+"; }
string Minus::str() const { return "-"; }
string Times::str() const { return "*"; }
string Divide::str() const { return "/"; }
string Power::str() const { return "^"; }
string Integer::str() const
{
    stringstream out;
    out << i;
    return out.str();
}
string Real::str() const
{
    stringstream out;
    out << d;
    return out.str();
}
string Variable::str() const
{
    return var;
}


string Binary_Operator::get_postfix() const
{
    return lhs->get_postfix().append(" "+rhs->get_postfix().append(" "+str()));
}

string Operand::get_postfix() const { return str(); }

string Binary_Operator::get_infix() const
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

string Operand::get_infix() const { return str(); }


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
    Expression_Tree* l_temp;
    Expression_Tree* r_temp;
    Expression_Tree* clone;
    try
    {
        l_temp = lhs->clone();
        r_temp = rhs->clone();
        clone = new Assign(l_temp, r_temp, var_table);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete l_temp;
        delete r_temp;
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Plus::clone()
{
    Expression_Tree* l_temp;
    Expression_Tree* r_temp;
    Expression_Tree* clone;
    try
    {
        l_temp = lhs->clone();
        r_temp = rhs->clone();
        clone = new Plus(l_temp, r_temp);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete l_temp;
        delete r_temp;
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Minus::clone()
{
    Expression_Tree* l_temp;
    Expression_Tree* r_temp;
    Expression_Tree* clone;
    try
    {
        l_temp = lhs->clone();
        r_temp = rhs->clone();
        clone = new Minus(l_temp, r_temp);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete l_temp;
        delete r_temp;
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Times::clone()
{
    Expression_Tree* l_temp;
    Expression_Tree* r_temp;
    Expression_Tree* clone;
    try
    {
        l_temp = lhs->clone();
        r_temp = rhs->clone();
        clone = new Times(l_temp, r_temp);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete l_temp;
        delete r_temp;
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Divide::clone()
{
    Expression_Tree* l_temp;
    Expression_Tree* r_temp;
    Expression_Tree* clone;
    try
    {
        l_temp = lhs->clone();
        r_temp = rhs->clone();
        clone = new Divide(l_temp, r_temp);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete l_temp;
        delete r_temp;
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Power::clone()
{
    Expression_Tree* l_temp;
    Expression_Tree* r_temp;
    Expression_Tree* clone;
    try
    {
        l_temp = lhs->clone();
        r_temp = rhs->clone();
        clone = new Power(l_temp, r_temp);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete l_temp;
        delete r_temp;
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Integer::clone()
{
    Expression_Tree* clone;
    try
    {
        clone = new Integer(i);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Real::clone()
{
    Expression_Tree* clone;
    try
    {
        clone = new Real(d);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

Expression_Tree* Variable::clone()
{
    Expression_Tree* clone;
    try
    {
        clone = new Variable(var, var_table);
        return clone;
    }
    catch (const bad_alloc& e)
    {
        delete clone;
        throw expression_tree_error("Clone failed");
    }
}

void Variable::set_value(double value) { var_table->set_value(var, value); }

double Variable::get_value() const { return var_table->get_value(var); }
