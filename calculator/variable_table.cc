#include "variable_table.h"
#include <map>
#include <iostream>

using namespace std;

bool Variable_Table::find(string var)
{
    if (var_table.find(var) == var_table.end())
        return false;
    return true;
}            

void Variable_Table::insert(string var, double value)
{
    if (find(var))
        cout << "nu blev sebbe sur\n";
    else
        var_table.insert(pair<string, double>(var, value));
} 

void Variable_Table::remove(string var)
{
   var_table.erase(var);
}

void Variable_Table::set_value(string var, double value)
{
    if (not find(var))
        cout << var << " does not exist\n";
    else
        var_table[var] = value;
}

double Variable_Table::get_value(string var)
{    
    if (not find(var))
       cout << "existerar ej\n";
   else
       var_table[var];
}

void Variable_Table::list(ostream& os)
{
    map<string, double>::iterator end = var_table.end();
    for (map<string, double>::iterator it = var_table.begin(); it != end; ++it)
    {
        os << it->first << ", " << it->second << "\n";
    } 
}

void Variable_Table::clear()
{
    var_table.clear();
}

bool Variable_Table::empty()
{
   return var_table.empty();
}   
