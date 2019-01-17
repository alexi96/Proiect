#pragma once

#include <mpi.h>
#include <string.h>
#include <iostream>
#include <math.h>

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

class MatrixPart {
private:
	Byte * data;
	unsigned int length;
	unsigned int offset;

	MatrixPart& operator=(const MatrixPart& copy);
public:
	MatrixPart(const unsigned int length, const unsigned int offset);

	MatrixPart(const MatrixPart& copy);

	~MatrixPart();

	unsigned int Length() const;

	Byte& operator[](const int index) const;

	MatrixPart& operator+(const MatrixPart& copy) const;

	friend ostream& operator<< (ostream& stream, const MatrixPart& number);
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
