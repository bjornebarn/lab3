/*
 * Expression_Tree.h    2011-06-28
 */
#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H
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
   virtual std::string      get_postfix() = 0;
   virtual std::string      str() = 0;
   virtual void             print(std::ostream&) = 0;
   virtual Expression_Tree* clone() = 0;
};

/*
 * Skelett för de direkta subklasserna. Inför datamedlemmar, om 
 * det är lämpligt (dvs de ska finnas i alla subklasser), och även
 * funktionalitet för sådana datamedlemmar, om det är möjligt och
 * funktionerna kan ärvas av subkasserna. Välj lämplig åtkomst-
 * specifikation (public, protected, private) för de medlemmar
 * du inför.
 */

class Binary_Operator : public Expression_Tree
{
};

class Operand : public Expression_Tree
{
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
};

class Plus : public Binary_Operator
{ 
};

class Minus : public Binary_Operator 
{
};

class Times : public Binary_Operator
{
};

class Divide : public Binary_Operator
{
};

class Power: public Binary_Operator
{
};

class Integer : public Operand
{
};

class Real : public Operand
{
};

class Variable : public Operand
{
};

#endif
