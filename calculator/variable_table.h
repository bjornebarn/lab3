#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include <map>
#include <iostream>

using namespace std;

class Variable_Table
{
    private:
        map<string, double> var_table;
    public:
        bool find(string) const;
        void insert(const string, double);
        void remove(const string);
        void set_value(const string, double);
        double get_value(string) const;
        void list(ostream&) const;
        void clear();
        bool empty() const;
};

#endif
