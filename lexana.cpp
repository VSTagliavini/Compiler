/*Este arquivo chama-se lexana porque realiza a
 *LEXical ANAlysis de um programa e para manter
 *o padrão de nomes de bibliotecas em C/C++
 */

#include<fstream>
#include<string>
#include<set>

using namespace std;

int line = 1, pos = 0;
char proximo;
set<string> comp_sp_char({":=", "<=", ">="});
set<char> sp_chars({'.', ';', ',', '(', ')', ':', '=', '<', '>', '+', '-', '*'});
set<string> res_words({		"AND",
							"BEGIN",
							"DIV",
							"DO",
							"ELSE",
							"END",
							"FUNCTION",
							"IF",
							"LABEL",
							"NOT",
							"OR",
							"PROCEDURE",
							"PROGRAM",
							"THEN",
							"VAR",
							"WHILE"});

ifstream input_lex;
ofstream output_lex;

void write_output(string a) {
	output_lex << noskipws << a << ' ';
}
void write_output(char a) {
	output_lex << noskipws << a;
}
bool PROXIMO() {
	/*Se for posível ler um caractere de input file o leia e armazene em proximo então retorne true,
	 *se não retorne false*/
		
	if (input_lex.good() && input_lex.peek() != EOF) {
		input_lex >> noskipws >> proximo;

		if (proximo == '\n') {
			line++;
			pos = 1;
		} else if (proximo == '\t')
			pos += 3;
		else {
			pos++;
			if (proximo >= 'a' && proximo <= 'z')
				proximo -= 32;
		}
		return true;
	}
	proximo = -1;
	return false;
}
string CODIGO(string atomo, int tipo) {
	switch (tipo) {
		case 0: return "SC_"+atomo; break;
		case 1: return "RW_"+atomo; break;
		case 2: return "ID_"+atomo; break;
		case 3: return "NU_"+atomo; break;
	}
	return atomo+"-"+to_string(tipo);
}
void ERRO(string error) {
	throw error;
}

string AnaliseLexica() {
	string atomo = "";
	while (proximo == '\n' || proximo == ' ' || proximo == '\t') {
		if (proximo != ' ' && proximo != -1)
			write_output(proximo);
		PROXIMO();
		if (proximo == -1)
			break;
	}
		

	if (sp_chars.find(proximo) != sp_chars.end()) {
		atomo = proximo;
		PROXIMO();
		if (atomo == ":" && proximo == '=') {
			atomo = ":=";
			PROXIMO();
		} else if (atomo == "<" && proximo == '=') {
			atomo = "<=";
			PROXIMO();
		} else if (atomo == ">" && proximo == '=') {
			atomo = ">=";
			PROXIMO();
		}
		return CODIGO(atomo, 0);
	} else if (proximo >= 'A' && proximo <= 'Z') {
		do {
			atomo += proximo;
			PROXIMO();
		} while((proximo >= 'A' && proximo <= 'Z') || (proximo >= '0' && proximo <= '9'));
		if (res_words.find(atomo) != res_words.end())
			return CODIGO(atomo, 1);
		else
			return CODIGO(atomo, 2);
	} else if (proximo >= '0' && proximo <= '9') {
		do {
			atomo += proximo;
			PROXIMO();
		} while (proximo >= '0' && proximo <= '9');
		if (proximo >= 'A' && proximo <= 'Z')
			ERRO("Expressao invalida na posicao "+to_string(pos)+" da linha "+to_string(line));
		return CODIGO(atomo, 3);
	} else {
		if (proximo == '\n' || proximo == '\t' || proximo == ' ' || proximo == -1)
			return "";
		ERRO("Caractere invalido na posicao "+to_string(pos)+" da linha "+to_string(line));
		return "";
	}
		
}

void lexana(string a, string b) {
	input_lex.open(a);
	output_lex.open(b);

	if (input_lex.good()) {
		PROXIMO();
		do {
			write_output(AnaliseLexica()+"");
		} while(input_lex.good());
	} else
		ERRO("Nao foi possivel acessar o arquivo de entrada");

	input_lex.close();
	output_lex.close();
}
