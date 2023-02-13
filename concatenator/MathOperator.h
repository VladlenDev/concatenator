#pragma once

#include <string>

struct MathOperator {
public:
	std::string sign;
	unsigned short priority;
	MathOperator(const char* str, unsigned short prior) {
		sign = str;
		priority = prior;
	}
};