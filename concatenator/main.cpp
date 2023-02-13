
#include <iostream>
#include "Concatenation.h"
#include "Calculator.h"

using namespace std;

int main() {
	string expression = "Expression to be solved";
	Calculator calc;	
	while (expression != "q") {
		cout << "expression: ";
		getline(cin, expression);
		if (expression == "q") {
			break;
		}
		calc.execute(expression);
	}
	return 0;
}