/*
 * Expression_Tree.cc    2011-06-28
 */
#include "Expression_Tree.h"
// Inkludera för allt som kommer att användas i denna fil!
using namespace std;

// Separata definitioner för för Expression_Tree-klasserna definieras här.


Assign::Assign(Variable& var, Expression_Tree& right)
{
    lhs = left;
    rhs = right;
}

Plus::Plus(Variable& var, Expression_Tree& right)
{
    lhs = left;
    rhs = right;
}

Minus::Minus(Variable& var, Expression_Tree& right)
{
    lhs = left;
    rhs = right;
}

Times::Times(Variable& var, Expression_Tree& right)
{
    lhs = left;
    rhs = right;
}

Divide::Divide(Variable& var, Expression_Tree& right)
{
    lhs = left;
    rhs = right;
}

Power::Power(Variable& var, Expression_Tree& right)
{
    lhs = left;
    rhs = right;
}

Integer::Integer(int x) { i = x; }

Real::Real(double y) { d = y; }

Variable::Variable(string str) { var = str; }

double Assign::evaluate()
{
    double temp = lhs.evaluate(); //Måste sätta temp till en variabel!!!
}

double Plus::evaluate()
{
    return lhs.evaluate() + rhs.evaluate();
}

double Minus::evaluate()
{
    return lhs.evaluate() - rhs.evaluate();
}

double Times::evaluate()
{
    return lhs.evaluate() * rhs.evaluate();
}

double Divide::evaluate()
{
    return lhs.evaluate() / rhs.evaluate();
}

double Power::evaluate()
{
    return pow(lhs.evaluate(), rhs.evaluate());
}

double Integer::evaluate() { return (double) i; }

double Real::evaluate() { return d; }

//double Variable::evaluate() { return var_list(var); }

string Assign::get_postfix()
{
    return ((rhs.get_postfix()).push_back("=")).push_front(lhs);
}

string Plus::get_postfix()
{
    return ((rhs.get_postfix()).push_back("+")).push_front(lhs);
}

string Minus::get_postfix()
{
    return ((rhs.get_postfix()).push_back("-")).push_front(lhs);
}    

string Times::get_postfix()
{
    return ((rhs.get_postfix()).push_back("*")).push_front(lhs);
}

string Divide::get_postfix()
{
    return ((rhs.get_postfix()).push_back("/")).push_front(lhs);
}

string Power::get_postfix()
{
    return ((rhs.get_postfix()).push_back("^")).push_front(lhs);
}    

string Integer::get_postfix()
{
    stringstream out;
    return out << i;
}

string Real::get_postfix()
{
    stringstream out;
    return out << d;
} 

string Variable::get_postfix() { return var; }

string Assign::str() { return "="; }
string Plus::str() { return "+"; }
string Minus::str() { return "-"; }
string Times::str() { return "*"; }
string Divide::str() { return "/"; }
string Power::str() { return "^"; }
string Integer::str() 
{ 
    stringstream out;
    return out << i; 
}
string Real::str() 
{ 
    stringstream out;
    return out << d;
}
string Variable::str() { return var; }

void Assign::print(ostream& os)
{
    rhs.print("  " << os);
    cout << " /\n=\n \\\n";
    lhs.print("  " << os);
}

void Plus::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n+\n \\\n";
    lhs.print("  " << os);
}    


void Minus::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n-\n \\\n";
    lhs.print("  " << os);
}    


void Times::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n*\n \\\n";
    lhs.print("  " << os);
}    


void Divide::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n/\n \\\n";
    lhs.print("  " << os);
}    


void Power::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n^\n \\\n";
    lhs.print("  " << os);
}    


void Integer::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n" << i << "\n \\\n";
    lhs.print("  " << os);
}    


void Real::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n" << d << "\n \\\n";
    lhs.print("  " << os);
}    


void Variable::print(ostream&)
{
    rhs.print("  " << os);
    cout << " /\n" << var << "\n \\\n";
    lhs.print("  " << os);
}    

