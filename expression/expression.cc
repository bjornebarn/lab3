/*
 * Expression.cc    2011-06-28
 */
#include "variable_table.h"
#include "expression.h"
#include "expression_tree.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

// IMPLEMENTERA STUBBARNA NEDAN KORREKT.

Expression::Expression(Expression_Tree* Temp_Tree)
{
    Exp_Tree = Temp_Tree;
}

Expression::Expression(const Expression& Temp_Tree)
{
    if (Temp_Tree.empty())
        Exp_Tree = 0;
    else
        Exp_Tree = Temp_Tree.Exp_Tree->clone();
}

Expression::~Expression() { delete Exp_Tree; }

class expression_error : public logic_error
{
    public:
        explicit expression_error(const string& what_arg) throw()
            : logic_error(what_arg) {}
};     

/*
 * evaluate()
 */
double Expression::evaluate() const
{
    return Exp_Tree->evaluate();
}

/*
 * get_postfix()
 */
std::string Expression::get_postfix() const
{
    return Exp_Tree->get_postfix();
}

/*
 *
 */
std::string Expression::get_infix() const
{
    return Exp_Tree->get_infix();
}

/*
 * empty()
 */
bool Expression::empty() const
{
    if (Exp_Tree == 0)
        return true;
    else
        return false;
}

/*
 * print_tree()
 */
void Expression::print_tree(std::ostream& os) const
{
    Exp_Tree->print(os);
}

/*
 * swap(other)
 */
void Expression::swap(Expression& exp)
{
    Expression_Tree* temp_ptr = Exp_Tree;
    if (exp.empty())
        Exp_Tree = 0;
    else
        Exp_Tree = exp.Exp_Tree;
    exp.Exp_Tree = temp_ptr;
}

/*
 * swap(x, y)
 */
void swap(Expression& exp1, Expression& exp2)
{
    exp1.swap(exp2);
}


// make_expression() definieras efter namnrymden nedan.

// Namrymden nedan innehåller intern kod för infix-till-postfix-omvandling
// och generering av uttrycksträd. En anonym namnrymd begränsar användningen
// av medlemmarna till denna fil.
namespace
{
    using std::vector;
    using std::map;
    using std::make_pair;
    using std::string;

    // Underlag för att skapa prioritetstabellerna, mm. Högre värde inom
    // input_prio respektive stack_prio anger inbördes prioritetsordning.
    // Högre värde i input_prio jämfört med motsvarande position i stack_prio
    // innebär högerassociativitet, det motsatta vänsterassociativitet.
    // (Flerteckenoperatorer kan också hanteras med denna representation)
    const char* ops[]      = { "^", "*", "/", "+", "-", "=" };
    const int input_prio[] = {  8,   5,   5,   3,   3,   2  };
    const int stack_prio[] = {  7,   6,   6,   4,   4,   1  };
    const int n_ops = sizeof ops / sizeof(char*);

    // Tillåtna operatorer. Används av make_postfix() och make_expression_tree().
    const vector<string> operators(ops, ops + n_ops);

    // Teckenuppsättningar för operander. Används av make_expression_tree().
    const string letters("abcdefghijklmnopqrstuvwxyz");
    const string digits("0123456789");
    const string integer_chars(digits);
    const string real_chars(digits + '.');
    const string variable_chars(letters);
    const string operand_chars(letters + digits + '.');

    // Hjälpfunktioner för att kategorisera lexikala element.
    bool is_operator(char token)
    {
        return find(operators.begin(), operators.end(), string(1, token)) != operators.end();
    }

    bool is_operator(const string& token)
    {
        return find(operators.begin(), operators.end(), token) != operators.end();
    }

    bool is_operand(const string& token)
    {
        return token.find_first_not_of(operand_chars) == string::npos;
    }

    bool is_integer(const string& token)
    {
        return token.find_first_not_of(integer_chars) == string::npos;
    }

    bool is_real(const string& token)
    {
        return token.find_first_not_of(real_chars) == string::npos;
    }

    bool is_identifier(const string& token)
    {
        return token.find_first_not_of(letters) == string::npos;
    }

    // Prioritetstabeller. Används av make_postfix().
    typedef map<string, int> priority_table;

    // Funktion för att initiera en prioritetstabell, givet de tillåtna operatorerna,
    // deras prioritieter (inkommandeprioritet eller stackprioritet, beroende på vilken
    // tabell som ska skapas) och antalet operatorer.
    priority_table init_priority_table(const char* ops[], const int* prio, const int n_ops)
    {
        priority_table pm;

        for (int i = 0; i < n_ops; ++i)
        {
            pm.insert(make_pair(string(ops[i]), prio[i]));
        }

        return pm;
    }

    // Prioritetstabellerna, en för inkommandeprioritet och en för stackprioritet.
    const priority_table input_priority = init_priority_table(ops, input_prio, n_ops);
    const priority_table stack_priority = init_priority_table(ops, stack_prio, n_ops);

    // format_infix() tar en sträng med ett infixuttryck och formaterar den så att
    // det finns ett mellanrum mellan varje symbol. Används av make_postfix(), där
    // denna formatering underlättar parsningen.

    std::string format_infix(const std::string& infix)
    {
        string::const_iterator bos = infix.begin();
        string::const_iterator eos = infix.end();
        string                 formated;

        for (string::const_iterator it = bos; it != eos; ++it)
        {
            if (is_operator(*it) || *it == '(' || *it == ')')
            {
                // Se till att det är ett mellanrum före en operator eller parentes
                if (it != bos && *(it - 1) != ' ' && *(formated.end() - 1) != ' ')
                    formated.append(1, ' ');
                formated.append(1, *it);
                // Se till att det är ett mellanrum efter en operator eller parentes
                if ((it + 1) != eos && *(it + 1) != ' ')
                    formated.append(1, ' ');
            }
            else
            {
                if (*it != ' ')
                    formated.append(1, *it);
                else if (it != bos && *(it - 1) != ' ')
                    formated.append(1, *it);
            }
        }
        return formated;
    }

    // make_postfix() tar en infixsträng och returnerar motsvarande postfixsträng.

    std::string make_postfix(const std::string& infix)
    {
        using std::stack;
        using std::string;
        using std::istringstream;
        using std::find;

        stack<string> operator_stack;
        string        token;
        string        previous_token = "";
        bool          last_was_operand = false;
        int           paren_count = 0;

        istringstream is(format_infix(infix));
        string        postfix;

        while (is >> token)
        {
            if (is_operator(token))
            {
                if (! last_was_operand || postfix.empty() || previous_token == "(")
                {
                    throw expression_error("operator där operand förväntades\n");
                }

                // Järnvägsalgoritmen
                while (! operator_stack.empty() &&
                        input_priority.find(token)->second <=
                        stack_priority.find(operator_stack.top())->second)
                {
                    postfix += operator_stack.top() + ' ';
                    operator_stack.pop();
                }
                operator_stack.push(token);
                last_was_operand = false;
            }
            else if (token == "(")
            {
                operator_stack.push(token);
                ++paren_count;
            }
            else if (token == ")")
            {
                if (paren_count == 0)
                {
                    throw expression_error("vänsterparentes saknas\n");
                }

                if (previous_token == "(" && ! postfix.empty())
                {
                    throw expression_error("tom parentes\n");
                }

                while (! operator_stack.empty() && operator_stack.top() != "(")
                {
                    postfix += operator_stack.top() + ' ';
                    operator_stack.pop();
                }

                if (operator_stack.empty())
                {
                    throw expression_error("högerparentes saknar matchande vänsterparentes\n");
                }
                operator_stack.pop();
                --paren_count;
            }
            else if (is_operand(token))
            {
                if (last_was_operand || previous_token == ")")
                {
                    throw expression_error("operand där operator förväntades\n");
                }

                postfix += token + ' ';
                last_was_operand = true;
            }
            else
            {
                throw expression_error("otillåten symbol\n");
            }

            previous_token = token;
        }

        if (postfix == "")
        {
            throw expression_error("tomt infixuttryck!\n");
        }

        if ((! last_was_operand) && (! postfix.empty()))
        {
            throw expression_error("operator avslutar\n");
        }

        if (paren_count > 0)
        {
            throw expression_error("högerparentes saknas\n");
        }

        while (! operator_stack.empty())
        {
            postfix += operator_stack.top() + ' ';
            operator_stack.pop();
        }

        if (! postfix.empty())
        {
            postfix.erase(postfix.size() - 1);
        }

        return postfix;
    }

    // make_expression_tree() tar en postfixsträng och returnerar ett motsvarande 
    // länkat träd av Expression_Tree-noder. Tänk på minnesläckage...

    Expression_Tree* make_expression_tree(const std::string& postfix, Variable_Table* var_table)
    {
        using std::stack;
        using std::string;
        using std::istringstream;

        stack<Expression_Tree*> tree_stack;
        string                  token;
        istringstream           ps(postfix);
        try {
            while (ps >> token)
            {
                if (is_operator(token))
                {
                    if (tree_stack.empty()) 
                    {
                        throw expression_error("felaktig postfix\n");
                    }
                    Expression_Tree* rhs = tree_stack.top();
                    tree_stack.pop();

                    if (tree_stack.empty()) 
                    {
                        throw expression_error("felaktig postfix\n");
                    }
                    Expression_Tree* lhs = tree_stack.top();
                    tree_stack.pop();

                    try {
                        if (token == "^")
                        {
                            tree_stack.push(new Power(lhs, rhs));
                        }
                        else if (token == "*")
                        {
                            tree_stack.push(new Times(lhs, rhs));
                        }
                        else if (token == "/")
                        {
                            tree_stack.push(new Divide(lhs, rhs));
                        }
                        else if (token == "+")
                        {
                            tree_stack.push(new Plus(lhs, rhs));
                        }
                        else if (token == "-")
                        {
                            tree_stack.push(new Minus(lhs, rhs));
                        }
                        else if (token == "=")
                        {
                            tree_stack.push(new Assign(lhs, rhs, var_table));
                        }
                    }
                    catch (...)
                    {
                        delete lhs;
                        delete rhs;
                        throw bad_alloc();
                    }
                }
                else if (is_integer(token))
                {
                    tree_stack.push(new Integer(atoi(token.c_str())));
                }
                else if (is_real(token))
                {
                    tree_stack.push(new Real(atof(token.c_str())));
                }
                else if (is_identifier(token))
                {
                    tree_stack.push(new Variable(token,var_table));
                }
                else
                {
                    throw expression_error("felaktig postfix\n");
                }

            }
            // Det ska bara finnas ett träd på stacken om postfixen är korrekt.

            if (tree_stack.empty())
            {
                throw expression_error("ingen postfix given\n");
            }

            if (tree_stack.size() > 1)
            {
                while (! tree_stack.empty())
                {
                    delete tree_stack.top();
                    tree_stack.pop();
                }
                throw expression_error("felaktig postfix\n");
            }



        }
        catch (const bad_alloc& e)
        {
            cerr << "Bad memory allocation. Removing allocated memory...";
            while (! tree_stack.empty())
            {
                delete tree_stack.top();
                tree_stack.pop();
            }
        }
        catch (const expression_error& e)
        {
            cerr << e.what();
            while (! tree_stack.empty())
            {
                delete tree_stack.top();
                tree_stack.pop();
            }
        }

        // Returnera trädet (stacken försvinner nu)
        return tree_stack.top();
    }
} // namespace

/*
 * make_expression()
 */
Expression* make_expression(const string& infix, Variable_Table* var_table)
{
    return new Expression(make_expression_tree(make_postfix(infix), var_table));
}
