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
#include <sstream>

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
    
    struct Token {
        T_Type type;
        int val; //For T_Type number
    };
    
    typedef std::vector<Token> Expr_Token;
    
    struct Tokenizer_output {
        Expr_Token value;
        std::string error;
        bool is_okay() {
            return error.empty(); //use for testing as well
        }
    };
    
    struct Postfix_output {
        int value;
        std::string error;
        bool is_okay() {
            return error.empty(); //use for testing as well
        }
        
    };
    
    
    class ExprEvaluator {
        
    private:
        
        //Helper Functions
        
        static bool is_operator(T_Type t);
        static int operator_weight(T_Type t);
        static bool is_space(char c);
        static bool is_digit(char c);
        static void fix_negation(Expr_Token& exp);
        static bool is_variable(char c);
        static std::string fill_variables(const std::string& expr, int level);
        
        template<class T>
        static T pop(std::vector<T>& stack);
        
        static Tokenizer_output tokenizer(const std::string& s);
        
        //Token evaluator
        static Postfix_output evaluate_postfix(const Expr_Token& exp);
        
    public:
        
        static Postfix_output evaluate_postfix(const std::string& expr);
        
        //--------------------------------Test Functions -----------
        static void test_postfix_eval();
        
        
        
    };
    
#endif /* ExprEvaluator_hpp */
