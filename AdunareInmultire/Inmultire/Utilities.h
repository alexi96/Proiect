#pragma once

#include <mpi.h>
#include <string.h>
#include <iostream>

using namespace std;

#define DEBUG

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

