#include <iostream>
#include "DFA.h"

std::string readExpressionFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Eroare la deschiderea fisierului " << filename << std::endl;
		return "";
	}

	std::string expression, cleanExpression;
	std::getline(file, expression);
	file.close();

	std::string validSpecialChars = "()*|.";
	for (char ch : expression) {
		if (isalnum(static_cast<unsigned char>(ch)) || validSpecialChars.find(ch) != std::string::npos) {
			cleanExpression += ch;
		}
		else {
			std::cerr << "Caracter invalid: " << ch << std::endl;
		}
	}

	return cleanExpression;
}

std::string convertToPostfix(const std::string& expression) {
	std::string withConcatenationDots = DFA::addConcatenationDots(expression);
	return DFA::toPostfix(withConcatenationDots);
}

void displayMenu() {
	std::cout << "1 - Afiseaza expresia" << std::endl;
	std::cout << "2 - Afiseaza forma poloneza a expresiei" << std::endl;
	std::cout << "3 - Afiseaza AFN-ul rezultat" << std::endl;
	std::cout << "4 - Afiseaza AFD-ul rezultat" << std::endl;
	std::cout << "5 - Verifica un cuvant" << std::endl;
	std::cout << "6 - Iesire" << std::endl;
}

int main() {
	std::string expression = readExpressionFromFile("expression.txt");
	if (expression.empty()) {
		return 1;
	}

	std::string postfixForm = convertToPostfix(expression);

	DFA afn = DFA::ObtainNFA(postfixForm);
	DFA afd = DFA::ConvertToDFA(afn);

	int choice;
	std::string word;

	do {
		displayMenu();
		std::cout << "Alege o optiune: ";
		std::cin >> choice;

		switch (choice) {
		case 1:
			std::cout << "Expresia initiala: " << expression << std::endl;
			break;
		case 2:
			std::cout << "Forma poloneza a expresiei: " << postfixForm << std::endl;
			break;
		case 3:
			std::cout << "AFN rezultat din expresie: " << postfixForm << std::endl;
			afn.PrintAutomaton("outDFA.txt");
			std::cout << "AFN a fost salvat in outDFA.txt" << std::endl;
			break;
		case 4:
			std::cout << "AFD din AFN: " << std::endl;
			afd.PrintAutomaton("outNFA.txt");
			std::cout << "AFD a fost salvat in outNFA.txt" << std::endl;
			break;
		case 5:
			std::cout << "Introdu un cuvant pentru a verifica daca e acceptat: ";
			std::cin >> word;
			if (afd.CheckWord(word)) {
				std::cout << "Cuvantul " << word << " e acceptat" << std::endl;
			}
			else {
				std::cout << "Cuvantul " << word << " nu e acceptat" << std::endl;
			}
			break;
		case 6:
			std::cout << "Iesire din program " << std::endl;
			break;
		default:
			std::cout << "Optiune invalida! Incearca din nou " << std::endl;
			break;
		}
	} while (choice != 6);

	return 0;
}