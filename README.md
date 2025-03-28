# 🌟 DFA Project - Deterministic Finite Automaton (DFA/NFA) 🌟

## 🎯 Description
This project implements a Deterministic Finite Automaton (DFA) and a Non-Deterministic Finite Automaton (NFA) based on a regular expression. The program reads the expression from a file, converts it to Polish notation (postfix), and then generates the corresponding automaton. Additionally, it allows checking if a word is accepted by the resulting automaton.

## 🛠️ Features
- **Read expression from file** 📝
- **Convert expression to Polish notation (postfix)** ➡️
- **Generate NFA from expression** 🔄
- **Generate DFA from NFA** 🔄
- **Verify if a word is accepted by the automaton** 🔍

## ⚙️ Compilation and Running

1. **Clone the repository** or download the source code.
2. **Build the project** using your preferred C++ compiler. For example:
   ```bash
   g++ -o DFA_project main.cpp
   ```
3. **Run the program**:
   ```bash
   ./DFA_project
   ```

## 📄 Usage

When the program runs, you will be presented with a menu:

```
1 - Display the expression
2 - Display the postfix expression
3 - Display the resulting NFA
4 - Display the resulting DFA
5 - Check a word
6 - Exit
```

### Options:
1. **Display the original expression** – Shows the regular expression read from the file.
2. **Display the postfix expression** – Shows the expression converted to Polish notation.
3. **Display the resulting NFA** – Displays and saves the generated NFA in a file `outDFA.txt`.
4. **Display the resulting DFA** – Displays and saves the generated DFA in a file `outNFA.txt`.
5. **Check a word** – Enter a word to check if it is accepted by the generated DFA.
6. **Exit** – Exit the program.

## 📂 Files
- **expression.txt** – The input file containing the regular expression.
- **outDFA.txt** – The output file containing the resulting NFA.
- **outNFA.txt** – The output file containing the resulting DFA.

## 🧑‍💻 Example

### Input:

1. Read regular expression from `expression.txt`:
   ```
   a(b|c)*d
   ```

2. Convert to postfix notation:
   ```
   abc|*d.
   ```

3. Generate NFA and DFA.

### Menu:

```
1 - Display the expression
2 - Display the postfix expression
3 - Display the resulting NFA
4 - Display the resulting DFA
5 - Check a word
6 - Exit
```

### Word Check:
Input a word to check if it’s accepted by the DFA (e.g., "abd").

Happy coding! 🚀