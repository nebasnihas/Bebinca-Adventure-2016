//
//  ExprEvaluator.cpp
//  expr_evaluator

#include "ExprEvaluator.hpp"

typedef std::vector<Token> Expr_Token;

bool ExprEvaluator::is_operator(T_Type t){
    switch(t){
        case T_Type::U_MINUS:
            return true;
        case T_Type::MULTIPLY:
            return true;
        case T_Type::MINUS:
            return true;
        case T_Type::PLUS:
            return true;
        case T_Type::DIVIDE:
            return true;
        default:
            return false;
    }
}

int ExprEvaluator::operator_weight(T_Type t){
    switch(t){
        case T_Type::U_MINUS:
            return 4;
        case T_Type::MULTIPLY:
            return 3;
        case T_Type::MINUS:
            return 2;
        case T_Type::PLUS:
            return 2;
        case T_Type::DIVIDE:
            return 3;
        default:
            return -1;
    }
}

void ExprEvaluator::fix_negation(Expr_Token &exp){
    
    //Fixing two cases: if first token is eg: -1
    //                  if the previous one is left parenthesis or operator
    
    if (exp[0].type == T_Type::MINUS) {
        exp[0].type = T_Type::U_MINUS;
    }
    for(int i = 1; i < exp.size(); ++i) {
        T_Type prev = exp[i - 1].type;
        if (exp[i].type == T_Type::MINUS) {
            if (prev == T_Type::LEFT_PARENTHESIS || is_operator(prev)) {
                exp[i].type = T_Type::U_MINUS;
            }
        }
    }
}

bool ExprEvaluator::is_space(char c){
    return c == ' ' ||
    c == '\n' ||
    c ==  '\r' ||
    c == '\t';
}

bool ExprEvaluator::is_digit(char c){
    return '0' <= c && c <= '9';
}

bool ExprEvaluator::is_variable(char c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

template<class T>
T ExprEvaluator::pop(std::vector<T> &stack){
    T pop_val = stack.back();
    stack.pop_back();
    return pop_val;
}

Tokenizer_output ExprEvaluator::tokenizer(const std::string &s){
    Expr_Token exp;
    
    for(int i = 0; i < s.size(); i++) {
        if (is_space(s[i])) {
            i++;
            while (i < s.size() && is_space(s[i])) {
                i++;
            }
            if (i < s.size()) {
                i--;
            }
        } else if (is_digit(s[i])) {
            std::string number = std::string(1, s[i]);
            i++;
            
            while (i < s.size() && is_digit(s[i])) {
                number += std::string(1, s[i]);
                i++;
            }
            
            if (i < s.size()) i--;
            exp.push_back(Token{T_Type::NUMBER, stoi(number)});
        }
        
        else if (s[i] == '(') {
            exp.push_back(Token{T_Type::LEFT_PARENTHESIS, 0});
        } else if (s[i] == ')') {
            exp.push_back(Token{T_Type::RIGHT_PARENTHESIS, 0});
        } else if (s[i] == '+') {
            exp.push_back(Token{T_Type::PLUS, 0});
        } else if (s[i] == '-') {
            exp.push_back(Token{T_Type::MINUS, 0});
        } else if (s[i] == '*') {
            exp.push_back(Token{T_Type::MULTIPLY, 0});
        } else if (s[i] == '/') {
            exp.push_back(Token{T_Type::DIVIDE, 0});
        } else {
            std::string msg = "Tokenizer: unknown character '" + std::string(1, s[i]) + "'";
            return Tokenizer_output{Expr_Token{}, msg};
        }
    }
    return Tokenizer_output{exp, ""};
}

std::string ExprEvaluator::fill_variables(const std::string &expr, int level){
    std::string one_var_exp = expr;
    
    std::string str_level;
    std::ostringstream sstream;
    sstream << level;
    str_level = sstream.str();
    
    for(int i=0; i< one_var_exp.size(); i++){
        if(is_variable(one_var_exp[i])){
            one_var_exp.replace(i,str_level.size()-1,str_level);
        }
    }
    
    return one_var_exp;
}

Postfix_output ExprEvaluator::evaluate_postfix(const Expr_Token &exp){
    std::vector<int> stack;
    bool ok = true;
    
    
    for (Token tok : exp) {
        if (tok.type == T_Type::NUMBER) {
            stack.push_back(tok.val);
        } else if (tok.type == T_Type::U_MINUS) {
            
            if (stack.size() < 1) {
                ok = false;
                break;
            }
            int a = pop(stack);
            stack.push_back(-a); //The number on top of stack must be negated
        } else {
            
            //Binary operations must have two inputs
            if (stack.size() < 2) {
                ok = false;
                break;
            }
            int a = pop(stack);
            int b = pop(stack);
            switch (tok.type) {
                case T_Type::PLUS:
                    stack.push_back(a + b);
                    break;
                case T_Type::MINUS:
                    stack.push_back(b - a);
                    break;
                case T_Type::MULTIPLY:
                    stack.push_back(a * b);
                    break;
                case T_Type::DIVIDE:
                    if (a == 0.0) {
                        return Postfix_output{0, "Division Err: Dividing by 0"};
                    }
                    stack.push_back(b / a);
                    break;
                default:
                    //Program desined to handle basic operations as seen in
                    //spells.yaml file
                    std::cout << "Evaluation Err: unknown token type '"
                    + std::string(1, char(tok.type)) + "\n";
                    break;
                    
                    
            }
        }
    }
    if (ok && stack.size() > 0) {
        return Postfix_output{stack[0], ""};
    } else {
        return Postfix_output{0, "Err: Stack is empty"};
    }
}

Postfix_output ExprEvaluator::evaluate_postfix(const std::string &expr){
    Tokenizer_output tok = tokenizer(expr);
    if(tok.is_okay()) {
        return evaluate_postfix(tok.value);
    }else {
        return Postfix_output{-1, tok.error};
    }
}


void ExprEvaluator::test_postfix_eval(){
    for(;;){
        std::cout << "Input Eqn : ";
        std::string inp;
        std::getline(std::cin,inp);
        std::cout << "\n The input was: " << inp << "\n";
        Postfix_output res = evaluate_postfix(inp);
        if (res.is_okay()) {
            std::cout << " The result is : " << res.value << "\n";
            
        } else {
            std::cout << "Error encountered: " << res.error << "\n";
        }
        
    }
}
