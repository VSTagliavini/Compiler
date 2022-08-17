/*Nome: Virgilio Santinho Tagliavini
 *RA: 201025558
 *Sistema operacional: Windows 10 Pro
 *Compilador: GCC
 **/

#include <iostream>

#include "lexana.cpp"
#include "synana.cpp"

//Alterar o arquivo lexana.cpp porque nele ocorre uma recursão infinita

int main() {
    string a, b, syn_ana;
    a = "Exemplo1_Trab2_Compiladores.txt";
    b = "Output.txt";
    try {
        lexana(a, b);
        syn_ana = synana(b);
	} catch (string e) {
		syn_ana = e;
	}
    cout << syn_ana << endl;
}
