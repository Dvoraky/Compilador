#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) 
{
    std::ifstream inputFile(argv[1]);
    if(!inputFile.is_open()) 
    {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return 1;
    }

    // Lê todo o conteúdo do arquivo
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string sourceCode = buffer.str();
    inputFile.close();

    try 
    {
        // Fase 1: Análise Léxica
        Lexer lexer(sourceCode);
        std::vector<Token> tokens = lexer.tokenize();
        // lexer.printTokens(tokens); // Função para imprimir os tokens

        // Fase 2: Análise Sintática
        Parser parser(tokens);
        parser.parse();

        std::cout << "Compilação concluída com sucesso!" << std::endl;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Erro durante a compilação: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
