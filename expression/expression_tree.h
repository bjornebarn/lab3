/*
 * expression_tree.h    2011-06-28
 */
#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H
#include <iosfwd>
#include <string>
#include <stdexcept>

/*
 * expression_tree_error kastas om fel uppst�r i en Expression_Tree-operation.
 * Ett diagnostiskt meddelande ska kunna skickas med.
 */
// ATT G�RA!

/*
 * Expression_Tree �r en abstrakt, polymorf basklass f�r alla tr�dnodklasser.
 */
class Expression_Tree
{
public:
   // Viktiga saker kvar att t�nka p�! Till exempel initiering, kopiering
   // och destruering! Det kan �ven t�nkas att n�gon modifiering beh�vs.

   virtual double           evaluate() = 0;
   virtual std::string      get_postfix() = 0;
   virtual std::string      str() = 0;
   virtual void             print(std::ostream&) = 0;
   virtual Expression_Tree* clone() = 0;
};

/*
 * Skelett f�r de direkta subklasserna. Inf�r datamedlemmar, om 
 * det �r l�mpligt (dvs de ska finnas i alla subklasser), och �ven
 * funktionalitet f�r s�dana datamedlemmar, om det �r m�jligt och
 * funktionerna kan �rvas av subklasserna. V�lj l�mplig �tkomst-
 * specifikation (public, protected, private) f�r de medlemmar
 * du inf�r.
 */

class Binary_Operator : public Expression_Tree
{
    protected: //Kanske?
        Expression_Tree* lhs;
        Expression_Tree* rhs;

    public:
        std::string      get_postfix();
        void             print(std::ostream&);
        Expression_Tree* clone();
};

class Operand : public Expression_Tree
{
    public:
        std::string      get_postfix();
        void             print(std::ostream&);
        Expression_Tree* clone();
};

/*
 * Skelett f�r de konkreta klasserna. Inf�r eventuella datamedlemmar
 * som beh�ver l�ggas till f�r varje specifik klass och �verskugga
 * virtuella funktioner som �nnu ej �verskuggats. V�lj l�mplig
 * �tkomstspecifikation (public, protected, private) f�r de 
 * medlemmar du inf�r.
 */

class Assign : public Binary_Operator
{
    public:
       Assign(Expression_Tree* left, Expression_Tree* right);  
       double           evaluate();          
       std::string      str();               

};

class Plus : public Binary_Operator
{
    public:
        Plus(Expression_Tree* left, Expression_Tree* right);
        double           evaluate();          
        std::string      str();               
};

class Minus : public Binary_Operator 
{
    public:
        Minus(Expression_Tree* left, Expression_Tree* right);
        double           evaluate();          
        std::string      str();               
};

class Times : public Binary_Operator
{
    public:
        Times(Expression_Tree* left, Expression_Tree* right);
        double           evaluate();          
        std::string      str();               
};

class Divide : public Binary_Operator
{
    public:
        Divide(Expression_Tree* left, Expression_Tree* right);
        double           evaluate();          
        std::string      str();               
};

class Power: public Binary_Operator
{
    public:
        Power(Expression_Tree* left, Expression_Tree* right);
        double           evaluate();          
        std::string      str();               
};

class Integer : public Operand
{
    private:
        int num_var;

    public:
        std::string      str();               
        Integer(int);
        double           evaluate();          
};

class Real : public Operand
{
    private:
        double num_var;

    public:
        std::string      str();               
        Real(double);
        double           evaluate();          
};

class Variable : public Operand
{
    private:
        std::string num_var;

    public:
        std::string      str();               
        Variable(std::string);
        double  evaluate();        
        void    set_value(double value);
        double  get_value();
};

#endif
