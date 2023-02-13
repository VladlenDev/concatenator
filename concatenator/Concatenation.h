#pragma once

#include <cmath>

long int concatenate(long int a, long int b) {
	int answer = -1;
	size_t bOrder = 0;
	for (size_t bNew = b; bNew != 0; bOrder++) {
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

class ConcLongInt {
private:
	long int value;
public:
	ConcLongInt(long int i): value(i) {}
	long int operator || (long int b) {
		return concatenate(this->value, b);
	}
};