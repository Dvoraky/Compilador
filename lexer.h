#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

enum class TokenType 
{
    BEGIN, END, CONST, TYPE, IGUAL, PONTO_VIRGULA, VIRGULA, 
    DOIS_PONTOS, VAR, INTEGER, REAL, CHAR, BOOLEAN, ARRAY, 
    RECORD, OF, ABRE_COLCHETE, FECHA_COLCHETE, FUNCTION, 
    PROCEDURE, ABRE_PARENTESE, FECHA_PARENTESE, WHILE,
    DO, IF, THEN, FOR, WRITE, READ, ATRIBUICAO, TO, ELSE,
    FALSE, TRUE, OPERADOR_LOGICO, OPERADOR_COMPARATIVO,
    OPERADOR_MATEMATICO, PONTO, IDENTIFICADOR, NUMERO,
    STRING, ERROR
};

struct Token 
{
    TokenType type;
    std::string value;
};

extern const std::unordered_map<std::string, TokenType> keywords;

class Lexer 
{
    public:
        Lexer(const std::string &source);
        std::vector<Token> tokenize();
        void printTokens(const std::vector<Token>& tokens);
        
    private:
        std::string source;
        size_t pos;
    
        Token tokenizeKeyword();
        Token tokenizeNumber();
        Token tokenizeOperator();
        Token tokenizeString();
    
        bool isLetter(char ch);
        bool isDigit(char ch);
        bool isAlphaNumeric(char ch);
        bool isOperator(char ch);
        char peekNext();
        
};

#endif
