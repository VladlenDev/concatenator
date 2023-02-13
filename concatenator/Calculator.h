#pragma once

#include <string>
#include <vector>
#include "MathOperator.h"

#define MATH_OPERATORS_PRIORITIES_COUNT 3

class Calculator {
private:
	std::string expression;
	std::string numbers;
	std::string operations;
	long int answer;
	std::vector<MathOperator> ops;

	void remove_spaces();
	void expr_correction();
	bool expression_iscorrect();
	long int calculate_operation(std::string op);
	long int calculate_simple_expression(std::string expr);
	long int calculate_expression(std::string expr);
public:
	Calculator();
	long int execute(std::string expr);
};