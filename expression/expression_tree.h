/*
 * expression_tree.h    2011-06-28
 */
#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H
#include "variable_table.h"
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

   virtual double           evaluate() const = 0;
   virtual std::string      get_postfix() const = 0;
   virtual std::string      get_infix() const = 0;
   virtual std::string      str() const = 0;
   virtual void             print(std::ostream&, std::string = "") const = 0;
   virtual Expression_Tree* clone() const = 0;
   virtual ~Expression_Tree(){}
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
    protected:
        Expression_Tree* lhs;
        Expression_Tree* rhs;
        Binary_Operator(Expression_Tree*,Expression_Tree*);

    public:
        ~Binary_Operator();
        std::string      get_postfix() const;
        std::string      get_infix() const;
        void             print(std::ostream&, std::string = "") const;
};

class Operand : public Expression_Tree
{
    public:
        std::string      get_postfix() const;
        std::string      get_infix() const;
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
    private:
        Variable_Table* var_table;
    public:
        Assign(Expression_Tree*, Expression_Tree*, Variable_Table*);
        double           evaluate() const;
        std::string      str() const;
        Expression_Tree* clone() const;
};

class Plus : public Binary_Operator
{
    public:
        Plus(Expression_Tree*, Expression_Tree*);
        double           evaluate() const;
        std::string      str() const;
        Expression_Tree* clone() const;
};

class Minus : public Binary_Operator 
{
    public:
        Minus(Expression_Tree*, Expression_Tree*);
        double           evaluate() const;
        std::string      str() const;
        Expression_Tree* clone() const;
};

class Times : public Binary_Operator
{
    public:
        Times(Expression_Tree*, Expression_Tree*);
        double           evaluate() const;
        std::string      str() const;
        Expression_Tree* clone() const;
};

class Divide : public Binary_Operator
{
    public:
        Divide(Expression_Tree*, Expression_Tree*);
        double           evaluate() const;
        std::string      str() const;
        Expression_Tree* clone() const;
};

class Power: public Binary_Operator
{
    public:
        Power(Expression_Tree*, Expression_Tree*);
        double           evaluate() const;
        std::string      str() const;
        Expression_Tree* clone() const;
};

class Integer : public Operand
{
    private:
        int i;

    public:
        Integer(int);
        double           evaluate() const;
        std::string      str() const;
        void             print(std::ostream&, std::string = "") const;
        Expression_Tree* clone() const;
};

class Real : public Operand
{
    private:
        double d;

    public:
        Real(double);
        double           evaluate() const;
        std::string      str() const;
        void             print(std::ostream&, std::string = "") const;
        Expression_Tree* clone() const;
};

class Variable : public Operand
{
    private:
        std::string var;
        Variable_Table* var_table;

    public:
        Variable(std::string, Variable_Table*);
        double  evaluate() const;
        std::string      str() const;
        void    set_value(double);
        double  get_value() const;
        void             print(std::ostream&, std::string = "") const;
        Expression_Tree* clone() const;
};

#endif
