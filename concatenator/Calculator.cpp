#include <iostream>
#include <algorithm>
#include <cmath>
#include "Calculator.h"
#include "Concatenation.h"

void Calculator::remove_spaces() {
	expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
	expression.push_back(' ');
}
void Calculator::expr_correction() {
	std::string buff;
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
bool Calculator::expression_iscorrect() {
	char current_symbol;
	size_t pos = -1;
	int lBracketCount = 0, rBracketCount = 0;
	std::string alphabet = " ()" + numbers + operations;
	//	correction
	remove_spaces();
	//	check if empty
	if (expression.length() == 0) {
		std::cout << "Empty expression. Enter something." << std::endl;
		return false;
	}
	expr_correction();
	//	check if symbols are correct
	for (pos = 0; pos < expression.length(); pos++) {
		current_symbol = expression.at(pos);
		if (alphabet.find(current_symbol) == std::string::npos) {
			std::cout << "Can not find \'" << current_symbol << "\' in alphabet. Symbols allowed:" << alphabet << std::endl;
			return false;
		}
	}
	//	check operators existance
	if (expression.find_first_of(operations) == std::string::npos) {
		std::cout << "Expression does not include any of operations. Operations known:" << std::endl << operations << std::endl;
		return false;
	}
	//	check numbers existance
	if (expression.find_first_of(numbers) == std::string::npos) {
		std::cout << "Expression does not include any of operands. Numbers are important for numeric result!" << std::endl;
		return false;
	}
	//	check first operand
	if (expression.find_first_of(numbers + "(-") != 0) {
		std::cout << "Expression misses first number." << std::endl;
		return false;
	}
	if (expression.at(0) == '-' && !isdigit(expression.at(1)) && expression.at(1) != '(') {
		std::cout << "Incorrect negative first number." << std::endl;
		return false;
	}
	//	check sequence after first operand
	for (pos = 1; pos < expression.length(); pos++) {
		//	check 1-symbol signed operations like +, -, *, /
		if (std::string("+-/*").find(expression.at(pos)) != std::string::npos &&
		(std::string(numbers + ')').find(expression.at(pos - 1)) == std::string::npos ||
		std::string(numbers + '(').find(expression.at(pos + 1)) == std::string::npos)) {
			if (!(expression.at(pos) == '-' && expression.at(pos - 1) == '(')) {
				std::cout << "Expression misses operand before/after " << expression.at(pos) << std::endl;
				return false;
			}
		}
		//	check 2-symbol signed operations like || (just concatenation, add another bellow)
		else if (expression.at(pos) == '|') {
			if (std::string(numbers + ')').find(expression.at(pos - 1)) == std::string::npos ||
			std::string(numbers + '(').find(expression.at(pos + 2)) == std::string::npos) {
				std::cout << "Expression misses operand before/after \'||\'." << std::endl;
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
				std::cout << "Expression has empty brackets." << std::endl;
				return false;
			}
		}
		else if (expression.at(pos) == ')') {
			rBracketCount++;
		}
	}
	if (lBracketCount != rBracketCount) {
		std::cout << "Expression misses bracket(s)." << std::endl;
		return false;
	}
	else if (lBracketCount != 0) {
		if (expression.find(')') < expression.find('(') || expression.at(expression.find_last_of("()")) == '(') {
			std::cout << "Expression misses bracket(s)." << std::endl;
			return false;
		}
	}
	//	if expression is correct
	return true;
}
long int Calculator::calculate_operation(std::string op) {
	long int answ = 0, fOperand = 0, lOperand = 0;
	size_t pos = -1, len = -1;
	std::string numBuff;
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
long int Calculator::calculate_simple_expression(std::string expr) {
	//	check if solved already
	if (expr.find_last_of(operations) == 0 || expr.find_last_of(operations) == std::string::npos) {
		return stol(expr);
	}
	long int answ = 0;
	size_t lPos = -1, rPos = -1, opPos = -1;
	std::string buff;
	std::string currOperation;
	//	very deep ass hard forbidden magic algorithm (something is wrong)
	for (size_t prior = 0; prior < MATH_OPERATORS_PRIORITIES_COUNT; prior++) {
		for (size_t i = 0; i < ops.size(); i++) {
			if (ops[i].priority == prior) {
				while ((opPos = expr.rfind(ops[i].sign)) != std::string::npos && opPos != 0) {
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
					buff = std::to_string(calculate_operation(currOperation = expr.substr(lPos, rPos - lPos + 1)));
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
long int Calculator::calculate_expression(std::string expr) {
	long int answ = 0;
	size_t pos = -1, len = -1, revPos = -1;
	std::string buff;
	//	rigth search and solving in-brackets expressions, reverting signs
	for (int step = 1; (pos = expr.rfind('(')) != std::string::npos; step++) {
		std::cout << step << " step: " << expr << "= ";
		//	get whatever is inside lastly-opened brackets
		for (len = 0; expr.at(pos + len) != ')'; len++) {}
		buff = expr.substr(pos + 1, len - 1);
		//	if it is an in-brackets expression, get answer
		if (buff.find_last_of(operations) != 0) {
			buff = std::to_string(calculate_simple_expression(buff));
		}
		//	if in-brackets value is negative, revert the value's and a previous part's sign (+/-)
		if (buff.find_last_of(operations) == 0) {
			if ((revPos = expr.find_last_of("+-(", pos - 1)) != std::string::npos) {
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
		std::cout << expr << std::endl;
	}
	//	get final answer
	answ = calculate_simple_expression(expr);
	std::cout << "final step: " << expr << "= " << answ << std::endl;
	return answ;
}
Calculator::Calculator() {
	//	0-init
	expression = "expression is not entered yet";
	numbers = "1234567890";
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
long int Calculator::execute(std::string expr) {
	//	introduction
	expression = expr;
	answer = 0;
	//	check expression
	if (!expression_iscorrect()) {
		return -1;
	}
	//	calculations
	answer = calculate_expression(expression);
	//	answer
	std::cout << "Result: " << expression << "= " << answer << std::endl;
	return answer;
}