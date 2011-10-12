#ifndef VARIABLE_TABLE.H
#define VARIABLE_TABLE.H

#include <map>
#include <iostream>


using namespace std;



class Variable_Table
{
    private:
        map<string, double> var_table; 
    public:
        bool find(string);
        void insert(string, double);
        void remove(string);
        void set_value(string, double);
        double get_value(string);
        void list(ostream&);
        void clear();
        bool empty();
};

#endif
