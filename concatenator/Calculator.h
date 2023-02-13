#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include "Concatenation.h"

#define MATH_OPERATORS_PRIORITIES_COUNT 3

using namespace std;

struct MathOperator {
public:
	string sign;
	unsigned short priority;
	MathOperator(const char* str, unsigned short prior) {
		sign = str;
		priority = prior;
	}
};

class Calculator {
private:
	string expression;
	string numbers;
	string operations;
	long int answer;
	bool is_solved;
	vector<MathOperator> ops;

	void remove_spaces() {
		expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
		expression.push_back(' ');
	}

	void expr_correction() {
		string buff;
		size_t pos = -1;
	//	first operand correction
		if (expression.length() > 2) {
			if (expression.at(0) == '-' && expression.at(1) == '0') {
				for (pos = 1; expression.at(pos) == '0' && isdigit(expression.at(pos + 1)) &&
					pos < expression.length() - 2; pos++) {
				}
				if (expression.at(pos) == '0') {
					buff.push_back('0');
				}
				else {
					buff.push_back('-');
					buff.push_back(expression.at(pos));
				}
			}
			else if (expression.at(0) == '0' && isdigit(expression.at(1))) {
				for (pos = 0; expression.at(pos) == '0' && isdigit(expression.at(pos + 1)) &&
					pos < expression.length() - 2; pos++) {
				}
				buff.push_back(expression.at(pos));
			}
			for (++pos; pos < expression.length(); pos++) {
				buff.push_back(expression.at(pos));
			}
			expression = buff;
			buff.clear();
		}
		buff.push_back(expression.at(0));	//	need to rewrite with first operand writing correction
	//	remaining part correction
		for (pos = 1; pos < expression.length(); pos++) {
			buff.push_back(expression.at(pos));
			if (pos + 1 < expression.length()) {
			//	| -> ||
				if (expression.at(pos) == '|' && expression.at(pos - 1) != '|' && expression.at(pos + 1) != '|') {
					buff.push_back('|');
				}
			//	000N -> N, N: (0..9)
				if (pos + 2 < expression.length() && !isdigit(expression.at(pos)) && expression.at(pos + 1) == '0') {
					while (expression.at(pos + 1) == '0' && isdigit(expression.at(pos + 2))) {
						pos++;
					}
				}
			//	(-0... -> (0...
				if (expression.at(pos + 1) == '0' && buff.at(buff.length() - 1) == '-' && buff.length() > 2) {
					if (buff.at(buff.length() - 2) == '(') {
						buff.pop_back();
					}
				}
			}
		}
		expression = buff;
	}

	bool expression_iscorrect() {
		char current_symbol;
		size_t pos = -1;
		int lBracketCount = 0, rBracketCount = 0;
		string alphabet = " ()" + numbers + operations;
	//	correction
		remove_spaces();
		//	check if empty
		if (expression.length() == 0) {
			cout << "Empty expression. Enter something." << endl;
			return false;
		}
		expr_correction();
	//	check if symbols are correct
		for (pos = 0; pos < expression.length(); pos++) {
			current_symbol = expression.at(pos);
			if (alphabet.find(current_symbol) == string::npos) {
				cout << "Can not find \'" << current_symbol << "\' in alphabet. Symbols allowed:" << endl << alphabet << endl;
				return false;
			}
		}
	//	check operators existance
		if (expression.find_first_of(operations) == string::npos) {
			cout << "Expression does not include any of operations. Operations known:" << endl << operations << endl;
			return false;
		}
	//	check numbers existance
		if (expression.find_first_of(numbers) == string::npos) {
			cout << "Expression does not include any of operands. Numbers are important for numeric result!" << endl;
			return false;
		}
	//	check first operand
		if (expression.find_first_of(numbers + "(-") != 0) {
			cout << "Expression misses first number." << endl;
			return false;
		}
		if (expression.at(0) == '-' && !isdigit(expression.at(1)) && expression.at(1) != '(') {
			cout << "Incorrect negative first number." << endl;
			return false;
		}
	//	check sequence after first operand
		for (pos = 1; pos < expression.length(); pos++) {
		//	check 1-symbol signed operations like +, -, *, /
			if (string("+-/*").find(expression.at(pos)) != string::npos &&
			(string(numbers + ')').find(expression.at(pos - 1)) == string::npos ||
			string(numbers + '(').find(expression.at(pos + 1)) == string::npos)) {
				if (!(expression.at(pos) == '-' && expression.at(pos - 1) == '(')) {
					cout << "Expression misses operand before/after " << expression.at(pos) << endl;
					return false;
				}
			}
		//	check 2-symbol signed operations like || (just concatenation, add another bellow)
			else if (expression.at(pos) == '|') {
				if (string(numbers + ')').find(expression.at(pos - 1)) == string::npos ||
				string(numbers + '(').find(expression.at(pos + 2)) == string::npos) {
					cout << "Expression misses operand before/after \'||\'." << endl;
					return false;
				}
				else {
					pos++;
				}
			}
		}
	//	check brackets closing
		for (pos = 0; pos < expression.length(); pos++) {
			if (expression.at(pos) == '(') {
				lBracketCount++;
			//	check empty brackets
				if (expression.at(pos + 1) == ')') {
					cout << "Expression has empty brackets." << endl;
					return false;
				}
			}
			else if (expression.at(pos) == ')') {
				rBracketCount++;
			}
		}
		if (lBracketCount != rBracketCount) {
			cout << "Expression misses bracket(s)." << endl;
			return false;
		}
		else if (lBracketCount != 0) {
			if (expression.find(')') < expression.find('(') || expression.at(expression.find_last_of("()")) == '(') {
				cout << "Expression misses bracket(s)." << endl;
				return false;
			}
		}
	//	if expression is correct
		return true;
	}

	long int calculate_operation(string op) {
		long int answ = 0, fOperand = 0, lOperand = 0;
		size_t pos = -1, len = -1;
		string numBuff;
	//	find first num
		pos = 0;
		if (op.at(pos) == '-') {
			numBuff.push_back(op.at(pos));
			pos++;
		}
		for (pos; isdigit(op.at(pos)); pos++) {
			numBuff.push_back(op.at(pos));
		}
		fOperand = stol(numBuff);
		numBuff.clear();
	//	find last num
		pos = op.find_last_of(numbers);
		for (len = 0; isdigit(op.at(pos - len)); len++) {}
		numBuff = op.substr(pos - len + 1, len);
		lOperand = stol(numBuff);
		numBuff.clear();
	//	recognize operator and calculate answer
		pos = op.find_last_of(operations);
		switch (op.at(pos)) {
		case '+':
			answ = fOperand + lOperand;
			break;
		case '-':
			answ = fOperand - lOperand;
			break;
		case '*':
			answ = fOperand * lOperand;
			break;
		case '/':
			answ = fOperand / lOperand;
			break;
		case '|':
			answ = concatenate(fOperand, lOperand);
			break;
		}
	//	return result
		return answ;
	}

	long int calculate_simple_expression(string expr) {
	//	check if solved already
		if (expr.find_last_of(operations) == 0 || expr.find_last_of(operations) == string::npos) {
			return stol(expr);
		}
		long int answ = 0;
		size_t lPos = -1, rPos = -1, opPos = -1;
		string buff;
		string currOperation;
	//	very deep ass hard forbidden magic algorithm (something is wrong)
		for (size_t prior = 0; prior < MATH_OPERATORS_PRIORITIES_COUNT; prior++) {
			for (size_t i = 0; i < ops.size(); i++) {
				if (ops[i].priority == prior) {
					while ((opPos = expr.rfind(ops[i].sign)) != string::npos && opPos != 0) {
						for (lPos = opPos; isdigit(expr.at(lPos - 1)) && lPos > 1; lPos--) {}
						if (lPos == 1 || expr.at(lPos - 1) == '-') {
							lPos--;
						}
						for (rPos = opPos + ops[i].sign.length(); isdigit(expr.at(rPos)) && rPos < expr.length() - 1; rPos++) {}
						if (rPos == expr.length() - 1) {
							rPos++;
						}
						else {
							rPos--;
						}
						buff = to_string(calculate_operation(currOperation = expr.substr(lPos, rPos - lPos + 1)));
						expr.replace(lPos, currOperation.length(), buff);
						if (currOperation.at(0) == '-' && buff.at(0) != '-') {
							expr.insert(lPos, "+");
						}
					}
				}
			}
		}
		answ = stol(buff);
		return answ;
	}

	long int calculate_expression(string expr) {
		long int answ = 0;
		size_t pos = -1, len = -1, revPos = -1;
		string buff;
	//	rigth search and solving in-brackets expressions, reverting signs
		for (int step = 1; (pos = expr.rfind('(')) != string::npos; step++) {
			cout << step << " step: " << expr << "= ";
		//	get whatever is inside lastly-opened brackets
			for(len = 0; expr.at(pos + len) != ')'; len++) {}
			buff = expr.substr(pos + 1, len - 1);
		//	if it is an in-brackets expression, get answer
			if (buff.find_last_of(operations) != 0) {
				buff = to_string(calculate_simple_expression(buff));
			}
		//	if in-brackets value is negative, revert the value's and a previous part's sign (+/-)
			if (buff.find_last_of(operations) == 0) {
				if ((revPos = expr.find_last_of("+-(", pos - 1)) != string::npos) {
					if (expr.at(revPos) == '+') {
						expr.replace(revPos, 1, "-");
					}
					else if (expr.at(revPos) == '-' && revPos > 0) {
						if (isdigit(expr.at(revPos - 1)) || expr.at(revPos - 1) == ')') {
							expr.replace(revPos, 1, "+");
						}
						else {
							expr.erase(revPos, 1);
							pos--;
						}
					}
					else if (expr.at(revPos) == '-' && revPos == 0) {
						expr.erase(revPos, 1);
						pos--;
					}
					else {
						expr.insert(revPos + 1, "-");
						pos++;
					}
				}
				else {
					expr.insert(0, "-");
					pos++;
				}
				buff.erase(0, 1);
			}
		//	insert the value in its position in upper expression
			expr.replace(pos, len + 1, buff);
			cout << expr << endl;
		}
	//	get final answer
		answ = calculate_simple_expression(expr);
		return answ;
	}

public:
	Calculator() {
	//	0-init
		expression = "expression is not entered yet";
		numbers = "1234567890";
		is_solved = true;
		answer = 0;
	//	adding operations
		ops.push_back(MathOperator("+", 2));
		ops.push_back(MathOperator("-", 2));
		ops.push_back(MathOperator("*", 1));
		ops.push_back(MathOperator("/", 1));
		ops.push_back(MathOperator("||", 0));
	//	fill string operations with operators signs
		for (size_t i = 0; i < ops.size(); i++) {
			operations += ops[i].sign;
		}
	}

	int execute(string expr) {
	//	introduction
		expression = expr;
		is_solved = false;
		answer = 0;
	//	check expression
		if (!expression_iscorrect()) {
			cout << "Wrong expression!" << endl << expression << endl;
			return -1;
		}
		else {
			cout << "Corrected expression: " << expression << ". Calculating..." << endl;
		}
	//	calculations
		answer = calculate_expression(expression);
	//	answer
		cout << "Result: " << expression << "= " << answer << endl;
		return 0;
	}
};
