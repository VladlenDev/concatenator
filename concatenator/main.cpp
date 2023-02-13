
#include <iostream>
#include "Calculator.h"

int main() {
	std::string expression = "Expression to be solved";
	Calculator calc;	
	while (expression != "q") {
		std::cout << "expression: ";
		getline(std::cin, expression);
		if (expression == "q") {
			break;
		}
		calc.execute(expression);
	}
	return 0;
}