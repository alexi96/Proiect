#pragma once

#include <mpi.h>
#include <string.h>
#include <iostream>

using namespace std;

typedef unsigned char Byte;

class Number {
private:
	Byte* data;
	unsigned int length;
public:
	Number(const char* const str);

	Number(const Number& copy);

	~Number();

	int operator[](int index) const;

	void Set(int index, int value);

	unsigned int Length() const;

	friend ostream& operator<< (ostream& stream, const Number& number);
};

template<typename N>
ostream& PrintAsNumber(N number, int len, ostream& stream = cout) {
	int index = 0;
	int t;
	for (; index < len; ++index) {
		t = (int) number[len - 1 - index];
		if (t != 0) {
			stream << t;
			++index;
			break;
		}
	}

	if (index == len) {
		stream << '0';
		return stream;
	}

	for (; index < len; ++index) {
		t = (int) number[len - 1 - index];
		stream << t;
	}
	return stream;
}
