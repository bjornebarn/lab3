/*
 * expression_tree-test.cc
 *
 * Program för att testa Expression_Tree-klasserna.
 */
#include "Expression_Tree.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
   // Ska inte vara möjligt - Expression_Tree ska vara abstrakt:
   //Expression_Tree* t0 = new Expression_Tree;

   Expression_Tree* t1 = new Plus(new Integer(7), new Real(3.14));

   try
   {
      cout << "t1->evaluate() = " << t1->evaluate() << '\n';
   }
   catch (const exception& e)
   {
      cout << "Undantag fångat: " << e.what() << '\n';
   }

   cout << "t1->get_postfix() = " << t1->get_postfix() << '\n';
   cout << "t1->str() = " << t1->str() << "\n\n";

   Expression_Tree* t2 = t1->clone();

   try
   {
      cout << "t2->evaluate() = " << t2->evaluate() << '\n';
   }
   catch (const exception& e)
   {
      cout << "Undantag fångat: " << e.what() << '\n';
   }
   cout << "t2->get_postfix() = " << t2->get_postfix() << '\n';
   cout << "t2->str() = " << t2->str() << "\n\n";

   return 0;
}
