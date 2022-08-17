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



//program <identificador> (<lista de identificadores>): <bloco> ;
void PROGRAM() {
    if (!isExpression("RW_PROGRAM"))
        SYN_ERRO("PROGRAM");
    PROX_SYN();
    if (!isIdentifier())
        SYN_ERRO("Identificador");
    PROX_SYN();
    while (isExpression("SC_,")) {
        PROX_SYN();
        if (!isIdentifier())
            SYN_ERRO("Identificador");
        PROX_SYN();
    }
    if (!isExpression("SC_:"))
        SYN_ERRO(":");
    PROX_SYN();
    BLOCO();
}

void BLOCO() {

}


string synana(string in) {
    input_syn.open(in);

    PROGRAM();

    input_syn.close();
    return "Análise sintática realizada com sucesso.";
}

