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


