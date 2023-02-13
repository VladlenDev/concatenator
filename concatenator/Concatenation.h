#pragma once

#include <cmath>

using namespace std;

long int concatenate(long int a, long int b) {
	int answer = -1, bOrder = 0;

	for (int bNew = b; bNew != 0; bOrder++) {
		bNew /= 10;
	}

	if (b == 0) {
		answer = a * 10;
	}
	else if (a >= 0 && b < 0) {
		answer = -a * pow(10, bOrder) + b;
	}
	else if (a < 0 && b > 0) {
		answer = a * pow(10, bOrder) - b;
	}
	else {
		answer = a * pow(10, bOrder) + b;
	}

	return answer;
}