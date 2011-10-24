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
 * expression_tree_error kastas om fel uppstår i en Expression_Tree-operation.
 * Ett diagnostiskt meddelande ska kunna skickas med.
 */
// ATT GÖRA!

/*
 * Expression_Tree är en abstrakt, polymorf basklass för alla trädnodklasser.
 */
class Expression_Tree
{
public:
   // Viktiga saker kvar att tänka på! Till exempel initiering, kopiering
   // och destruering! Det kan även tänkas att någon modifiering behövs.

   virtual double           evaluate() = 0;
   virtual std::string      get_postfix() const = 0;
   virtual std::string      get_infix() const = 0;
   virtual std::string      str() const = 0;
   virtual void             print(std::ostream&, std::string = "") = 0;
   virtual Expression_Tree* clone() = 0;
};

/*
 * Skelett för de direkta subklasserna. Inför datamedlemmar, om 
 * det är lämpligt (dvs de ska finnas i alla subklasser), och även
 * funktionalitet för sådana datamedlemmar, om det är möjligt och
 * funktionerna kan ärvas av subklasserna. Välj lämplig åtkomst-
 * specifikation (public, protected, private) för de medlemmar
 * du inför.
 */

class Binary_Operator : public Expression_Tree
{
    protected:
        Expression_Tree* lhs;
        Expression_Tree* rhs;

    public:
        Binary_Operator(Expression_Tree*,Expression_Tree*);
        ~Binary_Operator();
        std::string      get_postfix() const;
        std::string      get_infix() const;
        void             print(std::ostream&, std::string = "");
};

class Operand : public Expression_Tree
{
    public:
        std::string      get_postfix() const;
        std::string      get_infix() const;
};

/*
 * Skelett för de konkreta klasserna. Inför eventuella datamedlemmar
 * som behöver läggas till för varje specifik klass och överskugga
 * virtuella funktioner som ännu ej överskuggats. Välj lämplig
 * åtkomstspecifikation (public, protected, private) för de 
 * medlemmar du inför.
 */

class Assign : public Binary_Operator
{
    private:
        Variable_Table* var_table;
    public:
        Assign(Expression_Tree*, Expression_Tree*, Variable_Table*);
        ~Assign();
        double           evaluate();
        std::string      str() const;
        Expression_Tree* clone();
};

class Plus : public Binary_Operator
{
    public:
        Plus(Expression_Tree*, Expression_Tree*);
        double           evaluate();
        std::string      str() const;
        Expression_Tree* clone();
};

class Minus : public Binary_Operator 
{
    public:
        Minus(Expression_Tree*, Expression_Tree*);
        double           evaluate();
        std::string      str() const;
        Expression_Tree* clone();
};

class Times : public Binary_Operator
{
    public:
        Times(Expression_Tree*, Expression_Tree*);
        double           evaluate();
        std::string      str() const;
        Expression_Tree* clone();
};

class Divide : public Binary_Operator
{
    public:
        Divide(Expression_Tree*, Expression_Tree*);
        double           evaluate();
        std::string      str() const;
        Expression_Tree* clone();
};

class Power: public Binary_Operator
{
    public:
        Power(Expression_Tree*, Expression_Tree*);
        double           evaluate();
        std::string      str() const;
        Expression_Tree* clone();
};

class Integer : public Operand
{
    private:
        int i;

    public:
        Integer(int);
        ~Integer();
        double           evaluate();
        std::string      str() const;
        void             print(std::ostream&, std::string = "");
        Expression_Tree* clone();
};

class Real : public Operand
{
    private:
        double d;

    public:
        Real(double);
        ~Real();
        double           evaluate();
        std::string      str() const;
        void             print(std::ostream&, std::string = "");
        Expression_Tree* clone();
};

class Variable : public Operand
{
    private:
        std::string var;
        Variable_Table* var_table;

    public:
        Variable(std::string, Variable_Table*);
        ~Variable();
        double  evaluate();
        std::string      str() const;
        void    set_value(double);
        double  get_value() const;
        void             print(std::ostream&, std::string = "");
        Expression_Tree* clone();
};

#endif
