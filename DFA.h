#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <stack>
#include <cctype>
#include <vector>
#include <queue>
#include <regex>

struct Transition {
	std::string firstState;
	char secondState;
	std::string destState;

	bool operator<(const Transition& other) const {
		if (firstState == other.firstState) {
			if (secondState == other.secondState) {
				return destState < other.destState;
			}
			return secondState < other.secondState;
		}
		return firstState < other.firstState;
	}
};

class DFA
{
public:
	DFA(std::set<std::string> states, std::set<char> alphabet, std::set<Transition> transitions, std::string initialState, std::set<std::string> finalState);

	std::set<std::string> getStates() const;
	std::set<char> getAlphabet() const;
	std::set<Transition> getTransitions() const;
	std::string getInitialState() const;
	std::set<std::string> getFinalStates() const;

	bool VerifyAutomaton() const;
	bool isValidState(const std::string& state) const;
	bool isValidSymbol(char symbol) const;

	void PrintAutomaton(const std::string& fileName) const;

	bool CheckWord(const std::string& cuvant) const;

	bool isSymbolInAlphabet(char symbol) const;
	bool isFinalState(const std::string& currentState) const;
	bool FindValidTransition(const std::string& currentState, char symbol, std::string& nextState) const;

	static DFA alternate(const DFA& A, const DFA& B, int& count);
	static DFA concatenate(const DFA& A, const DFA& B);
	static DFA starring(const DFA& A, int& count);

	static std::string addConcatenationDots(const std::string& regex);
	static std::string toPostfix(std::string& regex);
	static std::set<std::string> lambdaClosure(const std::string& state, const std::map<std::string, std::vector<std::string>>& lambdaTransitions);

	static DFA ObtainNFA(const std::string& expression);
	static DFA ConvertToDFA(const DFA& afn);

private:

	void printStates(std::ostream& os) const;
	void printAlphabet(std::ostream& os) const;
	void printFinalStates(std::ostream& os) const;
	void printTransitions(std::ostream& os) const;

	std::set<std::string> states;
	std::set<char> alphabet;
	std::set<Transition> transitions;
	std::string initialState;
	std::set<std::string> finalState;

};

