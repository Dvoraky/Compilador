#include "parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

void Parser::parse() 
{
    programa();
    std::cout << "Parsing concluído com sucesso!" << std::endl;
}

const Token& Parser::currentToken() const 
{
    if(currentTokenIndex < tokens.size()) return tokens[currentTokenIndex];
    throw std::runtime_error("Fim inesperado do código fonte.");
}

void Parser::avance(TokenType expectedType) 
{
    if(currentToken().type == expectedType) currentTokenIndex++;
    else 
    {
        std::cout << currentTokenIndex << std::endl; 
        throw std::runtime_error("Token inesperado: " + currentToken().value);
    }
}

void Parser::programa() 
{
    declaracoes();
    bloco();
}

void Parser::bloco() 
{
    avance(TokenType::BEGIN);
    comando();
    listaCom();
    avance(TokenType::END);
}

void Parser::declaracoes() 
{
    defConst();
    defTipos();
    defVar();
    defRotina();
}

void Parser::defConst() 
{
    if(currentToken().type == TokenType::CONST) 
    {
        constante();
        defConst();
    }
}

void Parser::defTipos() 
{
    if(currentToken().type == TokenType::TYPE) 
    {
        tipo();
        defTipos();
    }
}

void Parser::defVar() 
{
    if(currentToken().type == TokenType::VAR) 
    {
        variavel();
        defVar();
    }
}

void Parser::defRotina() 
{
    if(currentToken().type == TokenType::FUNCTION || currentToken().type == TokenType::PROCEDURE) 
    {
        rotina();
        defRotina();
    }
}

void Parser::constante() 
{
    avance(TokenType::CONST);
    avance(TokenType::IDENTIFICADOR);
    avance(TokenType::IGUAL);
    constValor();
    avance(TokenType::PONTO_VIRGULA);
}

void Parser::constValor() 
{
    if(currentToken().type == TokenType::STRING)
    {
        avance(TokenType::STRING);
    }
    else
    {
        expConst();
    }
}

void Parser::tipo() 
{
    avance(TokenType::TYPE);
    avance(TokenType::IDENTIFICADOR);
    avance(TokenType::IGUAL);
    tipoDado();
    avance(TokenType::PONTO_VIRGULA);
}

void Parser::variavel() 
{
    avance(TokenType::VAR);
    campo();
    avance(TokenType::PONTO_VIRGULA);
}

void Parser::listaId() 
{
    if(currentToken().type == TokenType::VIRGULA) 
    {
        avance(TokenType::VIRGULA);
        avance(TokenType::IDENTIFICADOR);
        listaId();
    }
}

void Parser::campos() 
{
    campo();
    listaCampos();
}

void Parser::campo() 
{
    avance(TokenType::IDENTIFICADOR);
    listaId();
    avance(TokenType::DOIS_PONTOS);
    tipoDado();
}

void Parser::listaCampos() 
{
    if(currentToken().type == TokenType::PONTO_VIRGULA) 
    {
        avance(TokenType::PONTO_VIRGULA);
        campo();
        listaCampos();
    }
}

void Parser::tipoDado() 
{
    switch (currentToken().type) 
    {
        case TokenType::INTEGER:
            avance(TokenType::INTEGER);
            break;

        case TokenType::REAL:
            avance(TokenType::REAL);
            break;

        case TokenType::CHAR:
            avance(TokenType::CHAR);
            break;

        case TokenType::BOOLEAN:
            avance(TokenType::BOOLEAN);
            break;

        case TokenType::ARRAY:
            avance(TokenType::ARRAY);
            avance(TokenType::ABRE_COLCHETE);
            avance(TokenType::NUMERO);
            avance(TokenType::FECHA_COLCHETE);
            avance(TokenType::OF);
            tipoDado();
            break;

        case TokenType::RECORD:
            avance(TokenType::RECORD);
            campos();
            avance(TokenType::END);
            break;

        case TokenType::IDENTIFICADOR: 
            avance(TokenType::IDENTIFICADOR);
            break;

        default:
            throw std::runtime_error("Token inesperado em tipo de dado.");
    }
}

void Parser::rotina() 
{
    if(currentToken().type == TokenType::FUNCTION) 
    {
        avance(TokenType::FUNCTION);
        avance(TokenType::IDENTIFICADOR);
        paramRotina();
        avance(TokenType::DOIS_PONTOS);
        tipoDado();
        blocoRotina();
    } 
    else if(currentToken().type == TokenType::PROCEDURE) 
    {
        avance(TokenType::PROCEDURE);
        avance(TokenType::IDENTIFICADOR);
        paramRotina();
        blocoRotina();
    } 
    else 
    {
        throw std::runtime_error("Token inesperado em rotina.");
    }
}

void Parser::paramRotina()
{
    if(currentToken().type == TokenType::ABRE_PARENTESE)
    {
        avance(TokenType::ABRE_PARENTESE);
        campos();   
        avance(TokenType::FECHA_PARENTESE);
    }
}

void Parser::blocoRotina() 
{
    defVar();
    bloco();
}

void Parser::listaCom() 
{
    if(currentToken().type == TokenType::PONTO_VIRGULA) 
    {
        avance(TokenType::PONTO_VIRGULA);
        comando();
        listaCom();
    }
}

void Parser::blocoCom() 
{
    if(currentToken().type == TokenType::BEGIN) bloco();
    else comando();
}

void Parser::comando() 
{
    switch (currentToken().type) 
    {
        case TokenType::IDENTIFICADOR:
            avance(TokenType::IDENTIFICADOR); 
            nome(); 
            atribuicao();
            break;

        case TokenType::WHILE:
            avance(TokenType::WHILE);
            expCom();
            avance(TokenType::DO);
            blocoCom();
            break;

        case TokenType::IF:
            avance(TokenType::IF);
            expCom();
            avance(TokenType::THEN);
            blocoCom();
            ELSE();
            break;

        case TokenType::FOR:
            avance(TokenType::FOR);
            FOR();
            avance(TokenType::DO);
            blocoCom();
            break;

        case TokenType::WRITE:
            avance(TokenType::WRITE);
            constValor();
            break;

        case TokenType::READ:
            avance(TokenType::READ);
            avance(TokenType::IDENTIFICADOR);
            nome();
            break;

        default:
            throw std::runtime_error("Token inesperado no comando.");
    }
}

void Parser::FOR()
{
    avance(TokenType::IDENTIFICADOR);
    avance(TokenType::ATRIBUICAO);
    param();
    avance(TokenType::TO);
    param();
}

void Parser::ELSE()
{
    if(currentToken().type == TokenType::ELSE)
    {
        avance(TokenType::ELSE);
        blocoCom();
    }
}

void Parser::atribuicao()
{
    if(currentToken().type == TokenType::ATRIBUICAO)
    {
        avance(TokenType::ATRIBUICAO);
        exp();
    }
}

void Parser::lista_param()
{
    if(currentToken().type == TokenType::IDENTIFICADOR || currentToken().type == TokenType::NUMERO 
    || currentToken().type == TokenType::FALSE || currentToken().type == TokenType::TRUE)
    {
        param();
        if(currentToken().type == TokenType::VIRGULA)
        {
            avance(TokenType::VIRGULA);
            lista_param();
        }
    }
}

// void Parser::exp() 
// {   
//     if(currentToken().type == TokenType::ABRE_PARENTESE)
//     {
//         avance(TokenType::ABRE_PARENTESE);
//         param();
//         expL2();
//     }
//     else
//     {
//         param();
//         expL1();
//     }
// }

// Fiz uma mudança na regra para funcionar os dois códigos mas deixei o código antigo acima comentado
// e ele segue a gramática a risca, porém não funciona.

//Nova Regra:
// [EXP] -> [PARAMETRO] [OP_COMP] [PARAMETRO] [EXP_L1]
//          [PARAMETRO] [EXP_L1]

void Parser::exp() 
{
    param(); 
    if(currentToken().type == TokenType::OPERADOR_COMPARATIVO) 
    {
        opComp(); 
        param();  
    }
    expL1(); 
}

void Parser::expL1() 
{
    if(currentToken().type == TokenType::OPERADOR_MATEMATICO) 
    {
        opMat();
        exp();
    }
    else if(currentToken().type == TokenType::OPERADOR_LOGICO)
    {
        paramLogico();
        expLogica();
    }
}

void Parser::expLogica()
{
    if(currentToken().type == TokenType::OPERADOR_LOGICO)
    {
        opLogico();
        exp();
    }
}

void Parser::paramLogico()
{
    if(currentToken().type == TokenType::OPERADOR_COMPARATIVO)
    {
        opComp();
        param();
    }
}

void Parser::expL2() 
{
    if(currentToken().type == TokenType::OPERADOR_MATEMATICO)
    {
        opMat();
        exp();
        avance(TokenType::FECHA_PARENTESE);
    }
    else
    {
        paramLogico();
        opLogico();
        exp();
        avance(TokenType::FECHA_PARENTESE);
    }
}

void Parser::expConst() 
{
    if(currentToken().type == TokenType::ABRE_PARENTESE)
    {
        avance(TokenType::ABRE_PARENTESE);
        param();
        opMat();
        expConst();
        avance(TokenType::FECHA_PARENTESE);
    }
    else
    {
        param();
        expConstL();
    }
}

void Parser::expConstL() 
{
    if(currentToken().type == TokenType::OPERADOR_MATEMATICO) 
    {
        avance(TokenType::OPERADOR_MATEMATICO);
        expConst();
    }
}

void Parser::expCom() 
{
    if(currentToken().type == TokenType::ABRE_PARENTESE)
    {

        avance(TokenType::ABRE_PARENTESE);
        param();
        paramLogico();
        opLogico();
        expCom();
        avance(TokenType::FECHA_PARENTESE);
    }
    else
    {
        param();
        paramLogico();
        expComL();
    }
}

void Parser::expComL() 
{
    if(currentToken().type == TokenType::OPERADOR_LOGICO) 
    {
        opLogico();
        expCom();
    }
}

void Parser::param() 
{
    switch(currentToken().type) 
    {
        case TokenType::IDENTIFICADOR:
            avance(TokenType::IDENTIFICADOR); 
            nome(); 
            break;

        case TokenType::NUMERO:
            avance(TokenType::NUMERO);
            break;

        case TokenType::FALSE:
            avance(TokenType::FALSE);
            break;

        case TokenType::TRUE:
            avance(TokenType::TRUE);
            break;

        default:
            throw std::runtime_error("Token inesperado em parâmetro.");
    }
}

void Parser::opLogico() 
{
    avance(TokenType::OPERADOR_LOGICO);
}

void Parser::opComp() 
{
    avance(TokenType::OPERADOR_COMPARATIVO);   
}

void Parser::opMat() 
{
    avance(TokenType::OPERADOR_MATEMATICO);
}

void Parser::nome() 
{
    if(currentToken().type == TokenType::PONTO) 
    {
        avance(TokenType::PONTO);
        avance(TokenType::IDENTIFICADOR); 
        nome();
    }
    if(currentToken().type == TokenType::ABRE_COLCHETE) 
    {
        avance(TokenType::ABRE_COLCHETE);
        param();
        avance(TokenType::FECHA_COLCHETE);
    }
    else if(currentToken().type == TokenType::ABRE_PARENTESE)
    {
        avance(TokenType::ABRE_PARENTESE);
        lista_param();
        avance(TokenType::FECHA_PARENTESE);
    }
}
