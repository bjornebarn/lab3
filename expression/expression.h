/*
 * Expression.h
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "variable_table.h"
#include <iosfwd>
#include <stdexcept>
#include <string>
#include "expression_tree.h"
/**
 * expression_error kastas om fel intr�ffar i en Expression-operation.
 * Ett diagnostiskt meddelande kan skickas med.
 */
// ATT G�RA!

/**
 * Expression �r en klass f�r att representera ett enkelt aritmetiskt uttryck.
 */
class Expression
{
    private:
        Expression_Tree* Exp_Tree;
        Variable_Table* var_table;

    public:
        // VIKTIGA SAKER �TERST�R ATT FIXA OCH KANSKE MODIFIERA

        Expression(Expression_Tree* = 0);
        Expression(const Expression&);

        double      evaluate() const;
        std::string get_postfix() const;
        bool        empty() const;
        void        print_tree(std::ostream&) const;
        void        swap(Expression&);
};

void swap(Expression&, Expression&);

Expression make_expression(const std::string& infix, Variable_Table* var_table);

#endif
