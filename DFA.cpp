#include "DFA.h"

DFA::DFA(std::set<std::string> states, std::set<char> alphabet, std::set<Transition> transitions, std::string initialState, std::set<std::string> finalState)
	: states(states), alphabet(alphabet), transitions(transitions), initialState(initialState), finalState(finalState) {
}

std::set<std::string> DFA::getStates() const {
	return states;
}

std::set<char> DFA::getAlphabet() const {
	return alphabet;
}

std::set<Transition> DFA::getTransitions() const {
	return transitions;
}

std::string DFA::getInitialState() const {
	return initialState;
}

std::set<std::string> DFA::getFinalStates() const {
	return finalState;
}

bool DFA::VerifyAutomaton() const
{
	if (!isValidState(initialState)) {
		return false;
	}

	for (const auto& fin : finalState) {
		if (!isValidState(fin)) {
			std::cerr << "Stare finala invalida: " << fin << std::endl;
			return false;
		}
	}

	for (const auto& trans : transitions) {
		if (!isValidState(trans.firstState)) {
			std::cerr << "Stare sursa invalida: " << trans.firstState << std::endl;
			return false;
		}
		if (!isValidState(trans.destState)) {
			std::cerr << "Stare destinatie invalida: " << trans.destState << std::endl;
			return false;
		}
		if (!isValidSymbol(trans.secondState)) {
			std::cerr << "Simbol de tranzite invalid: " << trans.secondState << std::endl;
			return false;
		}
	}

	return true;
}

bool DFA::isValidState(const std::string& state) const {
	if (states.find(state) == states.end()) {
		std::cerr << "Stare invalida: " << state << std::endl;
		return false;
	}
	return true;
}

bool DFA::isValidSymbol(char symbol) const {
	if (alphabet.find(symbol) == alphabet.end()) {
		std::cerr << "Simbol invalid: " << symbol << std::endl;
		return false;
	}
	return true;
}

void DFA::printStates(std::ostream& os) const {
	os << "States: ";
	for (const auto& state : states) {
		os << state << ' ';
	}
	os << std::endl;
}

void DFA::printAlphabet(std::ostream& os) const {
	os << "Alphabet: ";
	for (const auto& symbol : alphabet) {
		os << symbol << ' ';
	}
	os << std::endl;
}

void DFA::printFinalStates(std::ostream& os) const {
	os << "Final States: ";
	for (const auto& state : finalState) {
		os << state << ' ';
	}
	os << std::endl;
}

void DFA::printTransitions(std::ostream& os) const {
	os << "Transitions: " << std::endl;
	for (const auto& transition : transitions) {
		os << "{" << transition.firstState << ", " << transition.secondState << "} -> " << transition.destState << std::endl;
	}
}

void DFA::PrintAutomaton(const std::string& fileName) const {
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		std::cerr << "Nu s a putut deschide fisierul " << fileName << std::endl;
		return;
	}
	printStates(std::cout);
	printAlphabet(std::cout);
	std::cout << "Initial State: " << initialState << std::endl;
	printFinalStates(std::cout);
	printTransitions(std::cout);

	printStates(fout);
	printAlphabet(fout);
	fout << "Initial State: " << initialState << std::endl;
	printFinalStates(fout);
	printTransitions(fout);

	fout.close();
}

bool DFA::CheckWord(const std::string& word) const {
	std::string currentState = initialState;

	for (char symbol : word) {
		if (!isSymbolInAlphabet(symbol)) {
			return false;
		}

		std::string nextState;
		if (!FindValidTransition(currentState, symbol, nextState)) {
			return false;
		}
		currentState = nextState;
	}

	if (isFinalState(currentState)) {
		std::cout << "Cuvantul e acceptat" << std::endl;
		return true;
	}
	else {
		std::cerr << "Cuvantul nu e acceptat, nu suntem intr-o stare finala" << std::endl;
		return false;
	}
}

bool DFA::isSymbolInAlphabet(char symbol) const {
	if (alphabet.find(symbol) == alphabet.end()) {
		std::cerr << "Simbolul " << symbol << " nu se afla in alfabet" << std::endl;
		return false;
	}
	return true;
}

bool DFA::isFinalState(const std::string& currentState) const {
	return finalState.find(currentState) != finalState.end();
}

bool DFA::FindValidTransition(const std::string& currentState, char symbol, std::string& nextState) const {
	for (const auto& tranzitie : transitions) {
		if (tranzitie.firstState == currentState && tranzitie.secondState == symbol) {
			nextState = tranzitie.destState;
			return true;
		}
	}
	std::cerr << "Nu exista o tranzitie din " << currentState << " pentru simbolul " << symbol << std::endl;
	return false;
}

DFA DFA::alternate(const DFA& A, const DFA& B, int& count) {
	std::set<std::string> states;
	std::set<char> alphabet;
	std::set<Transition> transitions;

	std::string newStart = "q" + std::to_string(count++);
	std::string newEnd = "q" + std::to_string(count++);

	for (const auto& state : A.getStates()) {
		states.insert(state);
	}
	for (const auto& state : B.getStates()) {
		states.insert(state);
	}
	states.insert(newStart);
	states.insert(newEnd);

	for (const auto& symbol : A.getAlphabet()) {
		alphabet.insert(symbol);
	}
	for (const auto& symbol : B.getAlphabet()) {
		alphabet.insert(symbol);
	}

	for (const auto& t : A.getTransitions()) {
		transitions.insert(Transition(t.firstState, t.secondState, t.destState));
	}
	for (const auto& t : B.getTransitions()) {
		transitions.insert(Transition(t.firstState, t.secondState, t.destState));
	}

	transitions.insert(Transition(newStart, '&', A.getInitialState()));
	transitions.insert(Transition(newStart, '&', B.getInitialState()));

	for (const auto& finalState : A.getFinalStates()) {
		transitions.insert(Transition(finalState, '&', newEnd));
	}
	for (const auto& finalState : B.getFinalStates()) {
		transitions.insert(Transition(finalState, '&', newEnd));
	}

	std::string initialState = newStart;
	std::set<std::string> finalState = { newEnd };

	return DFA(states, alphabet, transitions, initialState, finalState);
}

DFA DFA::concatenate(const DFA& A, const DFA& B) {
	std::set<std::string> states;
	std::set<char> alphabet;
	std::set<Transition> transitions;

	for (const auto& state : A.getStates()) {
		states.insert(state);
	}
	for (const auto& state : B.getStates()) {
		states.insert(state);
	}

	for (const auto& symbol : A.getAlphabet()) {
		alphabet.insert(symbol);
	}
	for (const auto& symbol : B.getAlphabet()) {
		alphabet.insert(symbol);
	}

	for (const auto& t : A.getTransitions()) {
		transitions.insert(Transition(t.firstState, t.secondState, t.destState));
	}
	for (const auto& t : B.getTransitions()) {
		transitions.insert(Transition(t.firstState, t.secondState, t.destState));
	}

	for (const auto& finalState : A.getFinalStates()) {
		transitions.insert(Transition(finalState, '&', B.getInitialState()));
	}

	std::string initialState = A.getInitialState();
	std::set<std::string> finalState = B.getFinalStates();

	return DFA(states, alphabet, transitions, initialState, finalState);
}

DFA DFA::starring(const DFA& A, int& count) {
	std::set<std::string> states;
	std::set<char> alphabet;
	std::set<Transition> transitions;

	std::string newStart = "q" + std::to_string(count++);
	std::string newEnd = "q" + std::to_string(count++);

	for (const auto& state : A.getStates()) {
		states.insert(state);
	}
	states.insert(newStart);
	states.insert(newEnd);

	for (const auto& symbol : A.getAlphabet()) {
		alphabet.insert(symbol);
	}

	for (const auto& t : A.getTransitions()) {
		transitions.insert(Transition(t.firstState, t.secondState, t.destState));
	}

	transitions.insert(Transition(newStart, '&', A.getInitialState()));
	transitions.insert(Transition(newStart, '&', newEnd));
	for (const auto& finalState : A.getFinalStates()) {
		transitions.insert(Transition(finalState, '&', newEnd));
		transitions.insert(Transition(finalState, '&', A.getInitialState()));
	}

	std::string initialState = newStart;
	std::set<std::string> finalState = { newEnd };

	return DFA(states, alphabet, transitions, initialState, finalState);
}

std::string DFA::addConcatenationDots(const std::string& regex) {
	std::string modifiedRegex;
	for (size_t i = 0; i < regex.size(); ++i) {
		char current = regex[i];
		modifiedRegex += current;

		if ((isalnum(current) || current == ')' || current == '*') &&
			(i + 1 < regex.size() && (isalnum(regex[i + 1]) || regex[i + 1] == '('))) {
			modifiedRegex += '.';
		}
	}
	return modifiedRegex;
}

std::string DFA::toPostfix(std::string& regex) {
	std::stack<char> operators;
	std::string result;

	auto precedence = [](char op) {
		if (op == '*') return 3;
		if (op == '.') return 2;
		if (op == '|') return 1;
		return 0;
		};

	int openBrackets = 0;

	for (char ch : regex) {
		if (isalnum(static_cast<unsigned char>(ch))) {
			result += ch;
		}
		else if (ch == '(') {
			operators.push(ch);
			openBrackets++;
		}
		else if (ch == ')') {
			openBrackets--;
			while (!operators.empty() && operators.top() != '(') {
				result += operators.top();
				operators.pop();
			}
			if (!operators.empty() && operators.top() == '(') {
				operators.pop();
			}
			else {
				std::cerr << "Paranteza inchisa fara pereche" << std::endl;
				return "";
			}
		}
		else if (ch == '*' || ch == '.' || ch == '|') {
			while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
				result += operators.top();
				operators.pop();
			}
			operators.push(ch);
		}
		else {
			std::cerr << "Caracter invalid detectat: " << ch << std::endl;
			return "";
		}
	}

	if (openBrackets != 0) {
		std::cerr << "Paranteze deschise fara pereche " << std::endl;
		return "";
	}

	while (!operators.empty()) {
		if (operators.top() == '(') {
			std::cerr << "Paranteza deschisa lipsa " << std::endl;
			return "";
		}
		result += operators.top();
		operators.pop();
	}

	return result;
}

std::set<std::string> DFA::lambdaClosure(const std::string& state, const std::map<std::string, std::vector<std::string>>& lambdaTransitions) {
	std::set<std::string> closure;
	std::queue<std::string> toVisit;

	closure.insert(state);
	toVisit.push(state);

	while (!toVisit.empty()) {
		std::string current = toVisit.front();
		toVisit.pop();

		if (lambdaTransitions.count(current)) {
			for (const auto& nextState : lambdaTransitions.at(current)) {
				if (!closure.count(nextState)) {
					closure.insert(nextState);
					toVisit.push(nextState);
				}
			}
		}
	}

	return closure;
}

DFA DFA::ObtainNFA(const std::string& expression) {
	std::stack<DFA> m_stack;
	int count = 0;

	for (char symb : expression) {
		if (isalnum(symb)) {
			std::set<std::string> states;
			std::set<char> alphabet;
			std::set<Transition> transitions;

			std::string initialState = "q" + std::to_string(count++);
			std::string finalState = "q" + std::to_string(count++);
			states.insert(initialState);
			states.insert(finalState);
			alphabet.insert(symb);
			transitions.insert(Transition(initialState, symb, finalState));

			std::set<std::string> finalStates = { finalState };
			DFA a(states, alphabet, transitions, initialState, finalStates);
			m_stack.push(a);
		}
		else if (symb == '|') {
			DFA B = m_stack.top(); m_stack.pop();
			DFA A = m_stack.top(); m_stack.pop();
			m_stack.push(DFA::alternate(A, B, count));
		}
		else if (symb == '.') {
			DFA B = m_stack.top(); m_stack.pop();
			DFA A = m_stack.top(); m_stack.pop();
			m_stack.push(DFA::concatenate(A, B));
		}
		else if (symb == '*') {
			DFA A = m_stack.top(); m_stack.pop();
			m_stack.push(DFA::starring(A, count));
		}
	}
	return m_stack.top();
}

DFA DFA::ConvertToDFA(const DFA& afn)
{
	auto afnStates = afn.getStates();
	auto afnAlphabet = afn.getAlphabet();
	auto afnTransitions = afn.getTransitions();
	auto afnInitialState = afn.getInitialState();
	auto afnFinalStates = afn.getFinalStates();

	std::map<std::string, std::vector<std::string>> lambdaTransitions;
	for (const auto& t : afnTransitions) {
		if (t.secondState == '&') {
			lambdaTransitions[t.firstState].push_back(t.destState);
		}
	}

	std::set<std::string> initialClosure = DFA::lambdaClosure(afnInitialState, lambdaTransitions);

	std::map<std::set<std::string>, std::string> stateMapping;
	std::set<std::string> afdStates;
	std::set<Transition> afdTransitions;
	std::string afdInitialState = "q0";
	std::set<std::string> afdFinalStates;

	stateMapping[initialClosure] = afdInitialState;
	afdStates.insert(afdInitialState);

	std::queue<std::set<std::string>> toProcess;
	toProcess.push(initialClosure);

	int stateCounter = 1;

	while (!toProcess.empty()) {
		std::set<std::string> current = toProcess.front();
		toProcess.pop();
		std::string currentStateName = stateMapping[current];

		for (const auto& s : current) {
			if (afnFinalStates.count(s)) {
				afdFinalStates.insert(currentStateName);
				break;
			}
		}

		for (char symbol : afnAlphabet) {
			std::set<std::string> newSet;

			for (const auto& state : current) {
				for (const auto& t : afnTransitions) {
					if (t.firstState == state && t.secondState == symbol) {
						std::set<std::string> closure = lambdaClosure(t.destState, lambdaTransitions);
						newSet.insert(closure.begin(), closure.end());
					}
				}
			}

			if (!newSet.empty()) {
				if (!stateMapping.count(newSet)) {
					stateMapping[newSet] = "q" + std::to_string(stateCounter++);
					afdStates.insert(stateMapping[newSet]);
					toProcess.push(newSet);
				}

				afdTransitions.insert(Transition(currentStateName, symbol, stateMapping[newSet]));
			}
		}
	}

	return DFA(afdStates, afnAlphabet, afdTransitions, afdInitialState, afdFinalStates);
}