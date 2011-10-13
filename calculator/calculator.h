/*
 * Calculator.h
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "variable_table.h"
#include "expression.h"
#include <iosfwd>
#include <vector>

/**
 * Calculator �r en klass f�r hantering av enkla aritmetiska uttryck.
 */

class Calculator
{

    private:
        // Kopiering och tilldelning till�ts ej (definieras ej)
        Calculator(const Calculator&);
        Calculator& operator=(const Calculator&);
        Variable_Table* var_table;
        vector<Expression> exp_vec;
        int cur_exp;

        static const std::string valid_command_;

        char command_;
        int  com_num;

        void print_help();
        void get_command();
        int  what_num();
        bool valid_command();
        void execute_command();

        void read_expression(std::istream&);

    public:
        // Defaultkonstruktorn m�ste deklareras, �ven om den "inget" g�r, eftersom
        // den inte genereras d� n�gon annan konstruktor deklareras.
        Calculator();
        ~Calculator();

        void run();
};

#endif
