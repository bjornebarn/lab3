/*
 * expression_tree.cc    2011-06-28
 */
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

Integer::Integer(int x) { i = x; }

Real::Real(double y) { d = y; }

Variable::Variable(string str) { var = str; }

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

double Integer::evaluate() { return (double) i; }

double Real::evaluate() { return d; }

double Variable::evaluate() { return 13.5; }
 
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
    Expression_Tree* clone = new Assign(lhs->clone(), rhs->clone());
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
    Expression_Tree* clone = new Variable(var);
    return clone;
}

 
//void Variable::set_value(double value) { set_list(num_var, value); }

//double Variable::get_value() { blalbalba ; }
//
