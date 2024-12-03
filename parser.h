#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <stdexcept>
#include <string>

class Parser 
{
    const std::vector<Token>& tokens;
    size_t currentTokenIndex = 0;

public:
    Parser(const std::vector<Token>& tokens);
    void parse();

private:
    const Token& currentToken() const;
    void avance(TokenType expectedType);

    void programa();
    void bloco();
    void declaracoes();
    void defConst();
    void defVar();
    void defRotina();
    void constante();
    void constValor();
    void tipo();
    void variavel();
    void listaId();
    void campos();
    void campo();
    void listaCampos();
    void defTipos();
    void tipoDado();
    void rotina();
    void paramRotina();
    void blocoRotina();
    void listaCom();
    void blocoCom();
    void comando();
    void FOR();
    void ELSE();
    void atribuicao();
    void lista_param();
    void exp();
    void expL1();
    void expLogica();
    void paramLogico();
    void expL2();
    void expConst();
    void expConstL();
    void expCom();
    void expComL();
    void param();
    void opLogico();
    void opComp();
    void opMat();
    void nome();
};

#endif
