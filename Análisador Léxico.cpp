#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <fstream>

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

const std::unordered_map<std::string, TokenType> keywords = {
    {"begin", TokenType::BEGIN}, {"end", TokenType::END}, {"const", TokenType::CONST},
    {"type", TokenType::TYPE}, {"var", TokenType::VAR}, {"integer", TokenType::INTEGER},
    {"real", TokenType::REAL}, {"char", TokenType::CHAR}, {"boolean", TokenType::BOOLEAN},
    {"array", TokenType::ARRAY}, {"record", TokenType::RECORD}, {"of", TokenType::OF},
    {"function", TokenType::FUNCTION}, {"procedure", TokenType::PROCEDURE},
    {"while", TokenType::WHILE}, {"do", TokenType::DO}, {"if", TokenType::IF},
    {"then", TokenType::THEN}, {"for", TokenType::FOR}, {"write", TokenType::WRITE},
    {"read", TokenType::READ}, {"to", TokenType::TO}, {"else", TokenType::ELSE},
    {"false", TokenType::FALSE}, {"true", TokenType::TRUE}
};

bool isLetter(char ch) 
{
    return std::isalpha(static_cast<unsigned char>(ch));
}

bool isDigit(char ch) 
{
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool isAlphaNumeric(char ch)
{
    return std::isalnum(static_cast<unsigned char>(ch));
}

class Lexer 
{
    public:
        Lexer(const std::string &source) : source(source), pos(0) {}

        std::vector<Token> tokenize() 
        {
            std::vector<Token> tokens;
            while(pos < source.length()) 
            {
                char currentChar = source[pos];

                if(std::isspace(currentChar))
                {
                    pos++;
                } 
                else if(isLetter(currentChar))
                {
                    tokens.push_back(tokenizeKeyword());
                } 
                else if(isDigit(currentChar))
                {
                    tokens.push_back(tokenizeNumber());
                } 
                else if(currentChar == '\'' || currentChar == '\"')
                {
                    tokens.push_back(tokenizeString());
                }
                else if(currentChar == ':' && peekNext() == '=')
                {
                    tokens.push_back({TokenType::ATRIBUICAO, ":="});
                    pos += 2;
                } 
                else if(currentChar == '<' && peekNext() == '=')
                {
                    tokens.push_back({TokenType::OPERADOR_COMPARATIVO, "<="});
                    pos += 2;
                } 
                else if(currentChar == '>' && peekNext() == '=')
                {
                    tokens.push_back({TokenType::OPERADOR_COMPARATIVO, ">="});
                    pos += 2;
                } 
                else if(currentChar == '!' && peekNext() == '=')
                {
                    tokens.push_back({TokenType::OPERADOR_COMPARATIVO, "!="});
                    pos += 2;
                } 
                else if(currentChar == '=' && peekNext() == '=')
                {
                    tokens.push_back({TokenType::OPERADOR_COMPARATIVO, "=="});
                    pos += 2;
                } 
                else if(currentChar == ':')
                {
                    tokens.push_back({TokenType::DOIS_PONTOS, ":"});
                    pos++;
                } 
                else if(currentChar == ';')
                {
                    tokens.push_back({TokenType::PONTO_VIRGULA, ";"});
                    pos++;
                } 
                else if(currentChar == ',')
                {
                    tokens.push_back({TokenType::VIRGULA, ","});
                    pos++;
                } 
                else if(currentChar == '(')
                {
                    tokens.push_back({TokenType::ABRE_PARENTESE, "("});
                    pos++;
                } 
                else if(currentChar == ')')
                {
                    tokens.push_back({TokenType::FECHA_PARENTESE, ")"});
                    pos++;
                } 
                else if(currentChar == '[')
                {
                    tokens.push_back({TokenType::ABRE_COLCHETE, "["});
                    pos++;
                } 
                else if(currentChar == ']')
                {
                    tokens.push_back({TokenType::FECHA_COLCHETE, "]"});
                    pos++;
                } 
                else if(currentChar == '.')
                {
                    tokens.push_back({TokenType::PONTO, "."});
                    pos++;
                } 
                else if(isOperator(currentChar))
                {
                    tokens.push_back(tokenizeOperator());
                } 
                else 
                {
                    tokens.push_back({TokenType::ERROR, std::string(1, currentChar)});
                    pos++;
                }
            }

            return tokens;
        }

    private:
        std::string source;
        size_t pos;

        Token tokenizeKeyword() 
        {
            std::string result;
            while(pos < source.length() && isAlphaNumeric(source[pos])) 
            {
                result += source[pos++];
            }

            if(keywords.find(result) != keywords.end()) 
            {
                return {keywords.at(result), result};
            } 
            return {TokenType::IDENTIFICADOR, result}; 
        }

        Token tokenizeString()
        {
            std::string result;
            while(pos < source.length())
            {
                result += source[pos++];
                if(source[pos] == '\'' || source[pos] == '"')
                {
                    result += source[pos++];
                    break;
                }
            }
            return {TokenType::STRING, result};
        }

        Token tokenizeNumber() 
        {
            int counter = 0;
            std::string result;
            while(pos < source.length() && (isDigit(source[pos]) || source[pos] == '.') && counter <= 1) 
            {
                result += source[pos++];
                if(source[pos] == '.') counter++;
            }

            if(counter <= 1) return {TokenType::NUMERO, result};
            else return{TokenType::ERROR, result};
        }

        Token tokenizeOperator() 
        {
            char currentChar = source[pos];
            std::string value(1, currentChar);
            pos++;

            if(currentChar == '>' || currentChar == '<' || currentChar == '=') 
            {
                return {TokenType::OPERADOR_COMPARATIVO, value};
            }
            else if(currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') 
            {
                return {TokenType::OPERADOR_MATEMATICO, value};
            }

            return {TokenType::ERROR, value};
        }

        bool isOperator(char ch) 
        {
            return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||  ch == '>' || ch == '<' || ch == '=');
        }   

        char peekNext() 
        {
            if(pos + 1 < source.length()) 
            {
                return source[pos + 1];
            }
            return '\0';
        }
};

void printTokens(const std::vector<Token>& tokens) 
{
    for(const auto &token : tokens)
    {
        std::cout << "Token: ";
        switch(token.type) 
        {
            case TokenType::BEGIN: std::cout << "BEGIN"; break;
            case TokenType::END: std::cout << "END"; break;
            case TokenType::CONST: std::cout << "CONST"; break;
            case TokenType::TYPE: std::cout << "TYPE"; break;
            case TokenType::IGUAL: std::cout << "IGUAL"; break;
            case TokenType::PONTO_VIRGULA: std::cout << "PONTO_VIRGULA"; break;
            case TokenType::VIRGULA: std::cout << "VIRGULA"; break;
            case TokenType::DOIS_PONTOS: std::cout << "DOIS_PONTOS"; break;
            case TokenType::VAR: std::cout << "VAR"; break;
            case TokenType::INTEGER: std::cout << "INTEGER"; break;
            case TokenType::REAL: std::cout << "REAL"; break;
            case TokenType::CHAR: std::cout << "CHAR"; break;
            case TokenType::BOOLEAN: std::cout << "BOOLEAN"; break;
            case TokenType::ARRAY: std::cout << "ARRAY"; break;
            case TokenType::RECORD: std::cout << "RECORD"; break;
            case TokenType::OF: std::cout << "OF"; break;
            case TokenType::ABRE_COLCHETE: std::cout << "ABRE_COLCHETE"; break;
            case TokenType::FECHA_COLCHETE: std::cout << "FECHA_COLCHETE"; break;
            case TokenType::FUNCTION: std::cout << "FUNCTION"; break;
            case TokenType::PROCEDURE: std::cout << "PROCEDURE"; break;
            case TokenType::ABRE_PARENTESE: std::cout << "ABRE_PARENTESE"; break;
            case TokenType::FECHA_PARENTESE: std::cout << "FECHA_PARENTESE"; break;
            case TokenType::WHILE: std::cout << "WHILE"; break;
            case TokenType::DO: std::cout << "DO"; break;
            case TokenType::IF: std::cout << "IF"; break;
            case TokenType::THEN: std::cout << "THEN"; break;
            case TokenType::FOR: std::cout << "FOR"; break;
            case TokenType::WRITE: std::cout << "WRITE"; break;
            case TokenType::READ: std::cout << "READ"; break;
            case TokenType::ATRIBUICAO: std::cout << "ATRIBUICAO"; break;
            case TokenType::TO: std::cout << "TO"; break;
            case TokenType::ELSE: std::cout << "ELSE"; break;
            case TokenType::FALSE: std::cout << "FALSE"; break;
            case TokenType::TRUE: std::cout << "TRUE"; break;
            case TokenType::OPERADOR_LOGICO: std::cout << "OPERADOR_LOGICO"; break;
            case TokenType::OPERADOR_COMPARATIVO: std::cout << "OPERADOR_COMPARATIVO"; break;
            case TokenType::OPERADOR_MATEMATICO: std::cout << "OPERADOR_MATEMATICO"; break;
            case TokenType::PONTO: std::cout << "PONTO"; break;
            case TokenType::IDENTIFICADOR: std::cout << "IDENTIFICADOR"; break;
            case TokenType::NUMERO: std::cout << "NUMERO"; break;
            case TokenType::STRING: std::cout << "STRING"; break;
            case TokenType::ERROR: std::cout << "ERROR"; break;
        }
        std::cout << " - " << token.value << std::endl;
    }
}

int main() 
{
    std::ifstream file("source.txt");
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();
    printTokens(tokens);

    return 0;
}
