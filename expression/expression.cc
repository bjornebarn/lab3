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

class expression_error : public logic_error
{
    public:
        explicit expression_error(const string& what_arg) throw()
            : logic_error(what_arg) {}
};     

Expression::Expression(Expression_Tree* Temp_Tree)
{
    Exp_Tree = Temp_Tree;
}

Expression::Expression(const Expression& Temp_Tree)
{
    if (Temp_Tree.empty())
        Exp_Tree = 0;
    else
    {
        try
        {
            Exp_Tree = Temp_Tree.Exp_Tree->clone();
        }
        catch (...)
        {
            delete Exp_Tree;
            throw expression_error("Clone failed");
        }
    }
}

Expression::~Expression() { delete Exp_Tree; }

Expression Expression::operator=(const Expression& Temp_Exp)
{
    Exp_Tree = Temp_Exp.Exp_Tree->clone();
    return *this;
}

/*
 * evaluate()
 */
double Expression::evaluate() const
{
    if (empty())
        return 0;
    return Exp_Tree->evaluate();
}

/*
 * get_postfix()
 */
std::string Expression::get_postfix() const
{
    if (empty())
        return "Tree is empty";
    return Exp_Tree->get_postfix();
}

/*
 *
 */
std::string Expression::get_infix() const
{
    if (empty())
        return "Tree is empty";
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
    if (not empty())     
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
    if (exp1.empty())
        exp1 = 0;
    exp1.swap(exp2);
}


// make_expression() definieras efter namnrymden nedan.

// Namrymden nedan inneh�ller intern kod f�r infix-till-postfix-omvandling
// och generering av uttryckstr�d. En anonym namnrymd begr�nsar anv�ndningen
// av medlemmarna till denna fil.
namespace
{
    using std::vector;
    using std::map;
    using std::make_pair;
    using std::string;

    // Underlag f�r att skapa prioritetstabellerna, mm. H�gre v�rde inom
    // input_prio respektive stack_prio anger inb�rdes prioritetsordning.
    // H�gre v�rde i input_prio j�mf�rt med motsvarande position i stack_prio
    // inneb�r h�gerassociativitet, det motsatta v�nsterassociativitet.
    // (Flerteckenoperatorer kan ocks� hanteras med denna representation)
    const char* ops[]      = { "^", "*", "/", "+", "-", "=" };
    const int input_prio[] = {  8,   5,   5,   3,   3,   2  };
    const int stack_prio[] = {  7,   6,   6,   4,   4,   1  };
    const int n_ops = sizeof ops / sizeof(char*);

    // Till�tna operatorer. Anv�nds av make_postfix() och make_expression_tree().
    const vector<string> operators(ops, ops + n_ops);

    // Teckenupps�ttningar f�r operander. Anv�nds av make_expression_tree().
    const string letters("abcdefghijklmnopqrstuvwxyz");
    const string digits("0123456789");
    const string integer_chars(digits);
    const string real_chars(digits + '.');
    const string variable_chars(letters);
    const string operand_chars(letters + digits + '.');

    // Hj�lpfunktioner f�r att kategorisera lexikala element.
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

    // Prioritetstabeller. Anv�nds av make_postfix().
    typedef map<string, int> priority_table;

    // Funktion f�r att initiera en prioritetstabell, givet de till�tna operatorerna,
    // deras prioritieter (inkommandeprioritet eller stackprioritet, beroende p� vilken
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

    // Prioritetstabellerna, en f�r inkommandeprioritet och en f�r stackprioritet.
    const priority_table input_priority = init_priority_table(ops, input_prio, n_ops);
    const priority_table stack_priority = init_priority_table(ops, stack_prio, n_ops);

    // format_infix() tar en str�ng med ett infixuttryck och formaterar den s� att
    // det finns ett mellanrum mellan varje symbol. Anv�nds av make_postfix(), d�r
    // denna formatering underl�ttar parsningen.

    std::string format_infix(const std::string& infix)
    {
        string::const_iterator bos = infix.begin();
        string::const_iterator eos = infix.end();
        string                 formated;

        for (string::const_iterator it = bos; it != eos; ++it)
        {
            if (is_operator(*it) || *it == '(' || *it == ')')
            {
                // Se till att det �r ett mellanrum f�re en operator eller parentes
                if (it != bos && *(it - 1) != ' ' && *(formated.end() - 1) != ' ')
                    formated.append(1, ' ');
                formated.append(1, *it);
                // Se till att det �r ett mellanrum efter en operator eller parentes
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

    // make_postfix() tar en infixstr�ng och returnerar motsvarande postfixstr�ng.

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
                    throw expression_error("operator d�r operand f�rv�ntades\n");
                }

                // J�rnv�gsalgoritmen
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
                    throw expression_error("v�nsterparentes saknas\n");
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
                    throw expression_error("h�gerparentes saknar matchande v�nsterparentes\n");
                }
                operator_stack.pop();
                --paren_count;
            }
            else if (is_operand(token))
            {
                if (last_was_operand || previous_token == ")")
                {
                    throw expression_error("operand d�r operator f�rv�ntades\n");
                }

                postfix += token + ' ';
                last_was_operand = true;
            }
            else
            {
                throw expression_error("otill�ten symbol\n");
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
            throw expression_error("h�gerparentes saknas\n");
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

    // make_expression_tree() tar en postfixstr�ng och returnerar ett motsvarande 
    // l�nkat tr�d av Expression_Tree-noder. T�nk p� minnesl�ckage...

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
            // Det ska bara finnas ett tr�d p� stacken om postfixen �r korrekt.

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
            while (! tree_stack.empty())
            {
                delete tree_stack.top();
                tree_stack.pop();
            }
            throw expression_error("Bad memory allocation.");
        }
        catch (const expression_error& e)
        {
            while (! tree_stack.empty())
            {
                delete tree_stack.top();
                tree_stack.pop();
            }
            throw e;
        }

        // Returnera tr�det (stacken f�rsvinner nu)
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
