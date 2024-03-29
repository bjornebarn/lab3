#include "variable_table.h"
#include <map>
#include <iostream>
#include <stdexcept>

using namespace std;

class variable_table_error : public logic_error
{
    public:
    explicit variable_table_error(const string& what_arg) throw()
        : logic_error(what_arg) {}
};

bool Variable_Table::find(string var) const
{
    if (var_table.find(var) == var_table.end())
        return false;
    return true;
}            

void Variable_Table::insert(const string var, double value)
{
    if (find(var))
        throw variable_table_error("Can not insert a variable that already exists!");
    else
        var_table.insert(pair<string, double>(var, value));
} 

void Variable_Table::remove(const string var)
{
   var_table.erase(var);
}

void Variable_Table::set_value(const string var, double value) 
{
    if (not find(var))
        throw variable_table_error("Can not set a new value to a var that isn't in the table!");
    else
        var_table[var] = value;
}

double Variable_Table::get_value(string var) const
{    
    if (not find(var))
        throw variable_table_error("Cannot get value of a var not in the table!");
    else
        return var_table.find(var)->second;
    return 0;
}

void Variable_Table::list(ostream& os) const
{
    map<string, double>::const_iterator end = var_table.end();
    for (map<string, double>::const_iterator it = var_table.begin(); it != end; ++it)
    {
        os << it->first << ": " << it->second << "\n";
    } 
}

void Variable_Table::clear()
{
    var_table.clear();
}

bool Variable_Table::empty() const
{
   return var_table.empty();
}   
