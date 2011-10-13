/*
 * Calculator.cc
 */
#include "variable_table.h"
#include "calculator.h"
#include "expression.h"
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <ctype.h>
#include <sstream>

using namespace std;

const string Calculator::valid_command_("?HUBPILTNARVXS");

Calculator::Calculator()
{
    var_table = new Variable_Table;
    cur_exp = -1;
}

Calculator::~Calculator() { delete var_table; }

/**
 * run() �r huvudfunktionen f�r kalkylatorn. Skriver f�rst ut hur man anv�nder
 * kalkylatorn och l�ser sedan sedan in ett kommando i taget och utf�r det.
 */
void Calculator::run()
{
   cout << "V�lkommen till Kalkylatorn!\n\n";
   print_help();

   do 
   {
      try 
      {
	 get_command();
	 if (valid_command()) execute_command();
      }
      catch (const exception& e) 
      {
	 cout << e.what() << '\n';
      }
      // Undantag som inte tillh�r exception avbryter programk�rningen!
   }
   while (command_ != 'S');
}

/**
 * print_help() skriver ut kommandorepertoaren.
 */
void Calculator::print_help()
{
   cout << "  H, ?  Skriv ut denna information\n";
   cout << "  U     Mata in ett nytt uttryck\n";
   cout << "  B     Ber�kna aktuellt uttryck\n";
   cout << "  P     Visa aktuellt uttryck som postfix\n";
   cout << "  T     Visa aktuellt uttryck som tr�d\n";
   cout << "  S     Avsluta kalkylatorn\n";
}

/**
 * get_command() l�ser ett kommando (ett tecken), g�r om till versal och lagrar
 * kommandot i medlemmen command_ f�r vidare behandling av andra operationer. 
 * Ingen kontroll g�rs om det skrivits mer, i s� fall skr�p, p� kommandoraden.
 */
void Calculator::get_command()
{
   string input;

   cout << ">> ";
   getline(cin,input);
   istringstream iss(input);
   iss >> command_;
   command_ = toupper(command_);
   if (input[1] == ' ')
   {
       iss >> com_num;
       com_num -= 1;
   }
   else
       com_num = -1;
}

int Calculator::what_num()
{
    int num;

    if (com_num < 0)
        num = cur_exp;
    else
        num = com_num;
    if (num < 0 || exp_vec.size() <= num)
        throw out_of_range("Inget uttryck finns!");
    return num;
}

/**
 * valid_command() kontrollerar om kommandot som finns i medlemmen command_
 * tillh�r den till�tna kommandorepertoraren och returnerar antingen true
 * (giltigt kommando) eller false (ogiltigt kommando).
 */
bool Calculator::valid_command()
{
   if (valid_command_.find(command_) == string::npos)
   {
      cout << "Otill�tet kommando: " << command_ << "\n";
      return false;
   }
   return true;
}

/**
 * execute_command() utf�r kommandot som finns i medlemmen command_. Kommandot
 * f�ruts�tts ha kontrollerats med valid_command() och allts� �r ett giltigt 
 * kommando.
 */
void Calculator::execute_command()
{
   if (command_ == 'H' || command_ == '?')
      print_help();
   else if (command_ == 'U')
      read_expression(cin);
   else if (command_ == 'B')
      cout << exp_vec[what_num()].evaluate() << "\n";
   else if (command_ == 'P')
      cout << exp_vec[what_num()].get_postfix() << "\n";
   else if (command_ == 'I')
      //cout << exp_vec[what_num()].get_infix() << "\n";
      cout << "Not implemented\n";
   else if (command_ == 'L')
      //cout << exp_vec[what_num()].get_infix() << "\n";
      cout << "Not implemented\n";
   else if (command_ == 'T')
      exp_vec[what_num()].print_tree(cout);
   else if (command_ == 'N')
      cout << "Antal uttryck: " << exp_vec.size() << "\n";
   else if (command_ == 'A')
      cur_exp = what_num();
   else if (command_ == 'R')
   {
       int num = what_num();
       exp_vec.erase(exp_vec.begin() + num);
       if (cur_exp >= num)
           cur_exp -= 1;
   }
   else if (command_ == 'V')
      var_table->list(cout);
   else if (command_ == 'X')
      var_table->clear();
   else if (command_ == 'S')
      cout << "Kalkylatorn avlutas, v�lkommen �ter!\n";
   else
      cout << "Detta ska inte h�nda!\n";
}

/**
 * read_expression() l�ser ett infixuttryck fr�n instr�mmen is och ger detta 
 * till funktionen make_expression() som returnerar ett objekt av typen 
 * Expression, vilket lagras som "aktuellt uttryck" i current_expression_.
 */
void Calculator::read_expression(istream& is)
{
   string infix;

   is >> ws;

   if (getline(is, infix))
   {
      exp_vec.push_back(make_expression(infix, var_table));
      cur_exp += 1;
   }
   else
   {
      cout << "Felaktig inmatning!\n";
   }
}
