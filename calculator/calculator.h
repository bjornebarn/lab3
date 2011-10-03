/*
 * Calculator.h
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "Expression.h"
#include <iosfwd>

/**
 * Calculator är en klass för hantering av enkla aritmetiska uttryck.
 */
class Calculator
{
public:
   // Defaultkonstruktorn måste deklareras, även om den "inget" gör, eftersom
   // den inte genereras då någon annan konstruktor deklareras.
   Calculator() {}

   void run();

private:
   // Kopiering och tilldelning tillåts ej (definieras ej)
   Calculator(const Calculator&);
   Calculator& operator=(const Calculator&);
      
   static const std::string valid_command_;

   Expression current_expression_;

   char command_;

   void print_help();
   void get_command();
   bool valid_command();
   void execute_command();

   void read_expression(std::istream&);
};

#endif
