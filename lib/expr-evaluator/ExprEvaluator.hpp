//
//  ExprEvaluator.hpp
//  expr_evaluator


#ifndef ExprEvaluator_hpp
#define ExprEvaluator_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>


enum class T_Type: char {
    
    NUMBER = 'n',
    LEFT_PARENTHESIS = '(',
    RIGHT_PARENTHESIS = ')',
    PLUS = '+',
    MINUS = 'm',
    U_MINUS = 'u', //The unary minus (eg: -3, 1 + -5)
    MULTIPLY = '*',
    DIVIDE = '/'
    
};


class ExprEvaluator {
   
private:
    

    struct Token {
        T_Type type;
        int val;
    };
    
    typedef std::vector<Token> Expr_Token;
    
    struct Tokenizer_output {
        Expr_Token value;
        std::string error;
        bool is_okay() {
            return error.empty(); //For testing
        }
    };
    
    //Helper Functions
    
    static bool is_operator(T_Type t);
    static int operator_weight(T_Type t);
    static bool is_space(char c);
    static bool is_digit(char c);
    static void fix_negation(Expr_Token& exp);
    
    static Tokenizer_output tokenizer(const std::string& s);
    
    
    
};

#endif /* ExprEvaluator_hpp */
