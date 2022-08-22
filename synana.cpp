/*Este arquivo chama-se lexana porque realiza a
 *SYNtactical ANAlysis de um programa e para manter
 *o padrão de nomes de bibliotecas em C/C++
 */

#include<fstream>
#include<string>
#include<set>

string prox_syn;
ifstream input_syn;

using namespace std;

bool PROX_SYN() {
    if (input_syn.good() && input_syn.peek() != EOF) {
        input_syn >> prox_syn;
        return true;
    }
    prox_syn = -1;
    return false;
}

void SYN_ERRO(string esp) {
    throw "Esperado " + esp + " mas encontrado " + prox_syn + '\n';
}

bool isIdentifier() {
    return prox_syn[0] == 'I';
}
bool isNumber() {
    return prox_syn[0] == 'N';
}
bool isSpecialCharacter() {
    return prox_syn[0] == 'S';
}
bool isReservedWord() {
    return prox_syn[0] == 'R';
}
bool isExpression(string a) {
    return prox_syn == a;
}

void PROGRAM();
void BLOCO();
void DefiniçaoDeTipo();
void Tipo();
void Indice();
void DeclaracaoDeVariaveis();
void PROCEDURE();
void FUNCTION();
void ParametrosFormais();
void SecaoParametros();
void ComandoComposto();
void Comando();
void ComandoSemRotulo();
void Expressao();
void ExpressaoSimples();
void Termo();
void Fator();

string synana(string in) {
    input_syn.open(in);

    PROX_SYN();

    PROGRAM();

    input_syn.close();
    return "Análise sintática realizada com sucesso.";
}

void PROGRAM() {
    //program
    if (!isExpression("RW_PROGRAM"))
        SYN_ERRO("PROGRAM");
    PROX_SYN();
    
    //<Identificador>
    if (!isIdentifier())
        SYN_ERRO("<identificador>");
    PROX_SYN();
    
    //(<Lista de identificadores>)
    while (isExpression("SC_,")) {
        PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("<identificador>");
        PROX_SYN();
    }
    
    //:
    if (!isExpression("SC_:"))
        SYN_ERRO(":");
    PROX_SYN();

    //<bloco>
    BLOCO();
    PROX_SYN();

    //.
    if (!isExpression("SC_."))
        SYN_ERRO(".");
}

void BLOCO() {
    //[Declarações de rótulos]
    if (isExpression("RW_LABEL")) {
        PROX_SYN();
        if (!isNumber())
            SYN_ERRO("<numero>");
        PROX_SYN();

        while (isExpression("SC_,")) {
            PROX_SYN();
            if (!isNumber())
                SYN_ERRO("<numero>");
            PROX_SYN();
        }

        if (!isExpression("SC_;"))
            SYN_ERRO(";");
        PROX_SYN();
    }
    //[Declarações de tipos]
    if (isExpression("RW_TYPE")) {
        PROX_SYN();
        
        DefiniçaoDeTipo();
        
        while (isExpression("SC_;")) {
            PROX_SYN();
            if (isIdentifier())
                DefiniçaoDeTipo();
        }
    }
    //[Declarações de variáveis]
    if (isExpression("RW_VAR")) {
        PROX_SYN();
        DeclaracaoDeVariaveis();
        while (isExpression("SC_;")) {
            PROX_SYN();
            if (isIdentifier())
                DeclaracaoDeVariaveis();
        }
    }
    //[Declarações de sub-rotinas]
    while (isExpression("RW_PROCEDURE") || isExpression("RW_FUNCTION")) {
        if (isExpression("RW_PROCEDURE"))
            PROCEDURE();
        else
            FUNCTION();
        if (!isExpression("SC_;"))
            SYN_ERRO(";");
        PROX_SYN();
    }

    ComandoComposto();
}

void DefiniçaoDeTipo() {
    //<Identificador>
    if (!isIdentifier())
        SYN_ERRO("<identificador>");
    PROX_SYN();
    //=
    if (!isExpression("SC_="))
        SYN_ERRO("=");
        PROX_SYN();
    //<Tipo>
    Tipo();
}

void Tipo() {
    //<Identificador>
    if (isIdentifier()) {
        PROX_SYN();
    }
    //of <Tipo>
    else if (isExpression("RW_OF")) {
        PROX_SYN();
        Tipo();
    }
    //array [ <Índice> {, <Índice>} ]
    else if (isExpression("RW_ARRAY")) {
        PROX_SYN();
        if (!isExpression("SC_["))
            SYN_ERRO("[");
        PROX_SYN();

        Indice();
        while (isExpression("SC_,"))
            Indice();

        if (!isExpression("SC_]"))
            SYN_ERRO("]");
        PROX_SYN();
    }
    else
        SYN_ERRO("<identificador> | ARRAY | OF");
}

void Indice() {
    //<Numero>
    if (!isNumber())
        SYN_ERRO("<numero>");
    PROX_SYN();
    //.
    if (!isExpression("SC_."))
        SYN_ERRO(".");
    PROX_SYN();
    //.
    if (!isExpression("SC_."))
        SYN_ERRO(".");
    PROX_SYN();
    //<Numero>
    if (!isNumber())
        SYN_ERRO("numero");
    PROX_SYN();
}

void DeclaracaoDeVariaveis() {
    //<Identificador>
    if (!isIdentifier())
        SYN_ERRO("<identificador>");
    PROX_SYN();
    //(, <Identificador>)
    while (isExpression("SC_,")) {
        PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("<identificador>");
        PROX_SYN();
    }
    //:
    if (!isExpression("SC_:"))
        SYN_ERRO(":");
    PROX_SYN();
    //<Tipo>
    Tipo();
}

void PROCEDURE() {
    //procedure
    if (!isExpression("RW_PROCEDURE"))
        SYN_ERRO("PROCEDURE");
    PROX_SYN();
    //<identificador>
    if (!isIdentifier())
        SYN_ERRO("<identificador>");
    PROX_SYN();
    //[parâmetros formais]
    if (isExpression("SC_("))
        ParametrosFormais();
    //;
    if (!isExpression("SC_;"))
        SYN_ERRO(";");
    PROX_SYN();
    //<bloco>
    BLOCO();
}

void FUNCTION() {
    //function
    if (!isExpression("RW_FUNCTION"))
        SYN_ERRO("FUNCTION");
    PROX_SYN();
    //<identificador>
    if (!isIdentifier())
        SYN_ERRO("<identificador>");
    PROX_SYN();
    //[parâmetros formais]
    if (isExpression("SC_("))
        ParametrosFormais();
    //:
    if (!isExpression("SC_:"))
        SYN_ERRO(":");
    PROX_SYN();
    //<identificador>
    if (!isIdentifier())
        SYN_ERRO("<identificador>");
    PROX_SYN();
    //;
    if (!isExpression("SC_;"))
        SYN_ERRO(";");
    PROX_SYN();
    //<bloco>
    BLOCO();
}

void ParametrosFormais() {
    //(
    if (!isExpression("SC_("))
        SYN_ERRO("(");
    PROX_SYN();
    //<seção de parâmetros formais>
    SecaoParametros();
    //{; <seção de parâmetros formais>}
    while (isExpression("SC_;")) {
        PROX_SYN();
        SecaoParametros();
    }
    //)
    if (!isExpression("SC_)"))
        SYN_ERRO(")");
    PROX_SYN();
}
void SecaoParametros() {
    //function <lista de identificadores> : <identificador>
    if (isExpression("RW_FUNCTION")) {
        PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("<identificador>");
        PROX_SYN();
        while (isExpression("SC_,")) {
            PROX_SYN();
            if (!isIdentifier())
                SYN_ERRO("<identificador>");
            PROX_SYN();
        }
        if (!isExpression("SC_:"))
            SYN_ERRO(":");
        PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("<identificador>");
        PROX_SYN();
    }
    //procedure <lista de identificadores>
    else if (isExpression("RW_PROCEDURE")) {
        PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("<identificador>");
        PROX_SYN();
        while (isExpression("SC_,")) {
            PROX_SYN();
            if (!isIdentifier())
                SYN_ERRO("<identificador>");
            PROX_SYN();
        }
    }
    //[var] <lista de identificadores> : <identificador>
    else if (isExpression("RW_VAR") || isIdentifier()) {
        if (isExpression("RW_VAR"))
            PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("<identificador>");
        PROX_SYN();
        while (isExpression("SC_,")) {
            PROX_SYN();
            if (!isIdentifier())
                SYN_ERRO("<identificador>");
            PROX_SYN();
        }
        if (!isExpression("SC_:"))
            SYN_ERRO(":");
        PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("<identificador>");
        PROX_SYN();
    } else
        SYN_ERRO("VAR | <identificador> | PROCEDURE | FUNCTION");
}
void ComandoComposto() {
    //begin
    if (!isExpression("RW_BEGIN"))
        SYN_ERRO("BEGIN");
    PROX_SYN();
    //<comando>
    Comando();
    //{<comando>}
    while (isNumber() | isIdentifier() || isExpression("RW_GOTO") || isExpression("RW_BEGIN") || isExpression("RW_IF") || isExpression("RW_WHILE"))
        Comando();
    //end
    if (!isExpression("RW_END"))
        SYN_ERRO("END");
    PROX_SYN();
}

void Comando() {
    //<numero> :
    if (isNumber()) {
        PROX_SYN();
        if (!isExpression("SC_:"))
            SYN_ERRO(":");
        PROX_SYN();
    }
    //<comando sem rotulo>
    ComandoSemRotulo();

    //if (!isExpression("SC_;"))
    //    SYN_ERRO(";");
    //PROX_SYN();
}
void ComandoSemRotulo() {
    bool aux = true;
    //<atribuição> | <chamada de procedimento>
    if (isIdentifier()) {
        PROX_SYN();
        if (isExpression("SC_:=")) {
            PROX_SYN();
            Expressao();
        } else if (isExpression("SC_(")) {
            PROX_SYN();
            Expressao();
            while (isExpression("SC_,")) {
                PROX_SYN();
                Expressao();
            }
            if (!isExpression("SC_)"))
                SYN_ERRO(")");
            PROX_SYN();
        } else if (!isExpression("SC_;"))
            SYN_ERRO("<identificador> | ( | ;");
    }
    //<desvio>
    else if (isExpression("RW_GOTO")) {
        PROX_SYN();
        if (!isNumber())
            SYN_ERRO("<numero>");
        PROX_SYN();
    }
    //<comando composto>
    else if (isExpression("RW_BEGIN")) {
        aux = false;
        ComandoComposto();
    }
    //<comando condicional>
    else if (isExpression("RW_IF")) {
        aux = false;
        PROX_SYN();
        Expressao();
        if (!isExpression("RW_THEN"))
            SYN_ERRO("THEN");
        PROX_SYN();
        ComandoSemRotulo();
        if (isExpression("RW_ELSE")) {
            PROX_SYN();
            ComandoSemRotulo();
        }
    }
    //<comando repetitivo>
    else if (isExpression("RW_WHILE")) {
        PROX_SYN();
        Expressao();
        if(!isExpression("RW_DO"))
            SYN_ERRO("DO");
        PROX_SYN();
        ComandoSemRotulo();
    } else
        SYN_ERRO("<identificador> | GOTO | BEGIN | IF | WHILE");
    if (aux) {
        if (!isExpression("SC_;"))
            SYN_ERRO(";");
        PROX_SYN();
    }
}

void Expressao() {
    ExpressaoSimples();
    bool aux = isExpression("SC_=") || isExpression("SC_<>") || isExpression("SC_<") || isExpression("SC_<=") || isExpression("SC_>") || isExpression("SC_>=");
    if (aux) {
        PROX_SYN();
        ExpressaoSimples();
    }
}

void ExpressaoSimples() {
    if (isExpression("SC_-") || isExpression("SC_+"))
        PROX_SYN();
    Termo();
    if (isExpression("SC_-") || isExpression("SC_+") || isIdentifier() || isNumber() || isExpression("SC_(") || isExpression("RW_NOT")) {
        PROX_SYN();
        Termo();
    }
}

void Termo() {
    Fator();
    if (isExpression("SC_*") || isExpression("RW_DIV") || isExpression("AND")) {
        PROX_SYN();
        Fator();
    }
}

void Fator() {
    if (isNumber()) {
        PROX_SYN();
    } else if (isExpression("SC_(")) {
        PROX_SYN();
        Expressao();
        if (!isExpression("SC_)"))
            SYN_ERRO(")");
        PROX_SYN();
    } else if (isExpression("RW_NOT")) {
        PROX_SYN();
        Fator();
    } else if (isIdentifier()) {
        PROX_SYN();
        bool aux = isExpression("SC_(");
        if (aux)
            PROX_SYN();
        bool aux2 = isExpression("SC_-") || isExpression("SC_+") || isIdentifier() || isIdentifier() || isExpression("RW_NOT") || isExpression("SC_(");
        if (aux2) {
            Expressao();
            while (isExpression("SC_,")) {
                PROX_SYN();
                Expressao();
            }
        }
        if (aux)
            PROX_SYN();
    } else
        SYN_ERRO("<identificador> | <numero> | () | NOT");
}