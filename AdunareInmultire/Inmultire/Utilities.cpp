#include "Utilities.h"

Number::Number(const char* const string) {
	int stringLength = strlen(string);
	this->length = stringLength / 2;
	if (stringLength % 2) {
		this->length = (stringLength + 1) / 2;
	}
	this->data = new Byte[this->length];
	for (int i = 0; i < this->length; ++i) {
		data[i] = 0;
	}

	for (int i = 0; i < stringLength; ++i) {
		this->Set(i, string[stringLength - 1 - i] - '0');
	}
}

Number::Number(const Number& copy) : length(copy.length) {
	this->data = new Byte[this->length];

	for (unsigned int i = 0; i < this->length; ++i) {
		this->data[i] = copy.data[i];
	}
}

Number::~Number() {
	delete this->data;
	this->data = nullptr;
}

int Number::operator[](const int index) const {
	Byte res = this->data[index / 2];
	if (index % 2) {
		res >>= 4;
	} else {
		res &= 0x0f;
	}

	return res;
}

void Number::Set(int index, int value) {
	if (value < 0 && value > 9) {
		return;
	}

	Byte& res = this->data[index / 2];
	if (index % 2) {
		res &= 0x0F;
		res |= value << 4;
	} else {
		res &= 0xF0;
		res |= value;
	}
}

unsigned int Number::Length() const {
	int res = this->length * 2;
	int t = (*this)[res - 1];
	res -= t == 0;
	return res;
}

ostream& operator<<(ostream& stream, const Number& number)
{
	int index = 0;
	int len = number.Length();
	int t;
	for (unsigned int i = 0; i < len; ++i) {
		t = (int) number[len - 1 - i];
	
		if (i == len - 1 && !t) {
			break;
		}

		stream << t;
	}
	return stream;
}

MatrixPart::MatrixPart(const unsigned int length, const unsigned int offset) : length(length), offset(offset) {
	this->data = new Byte[this->length];

	for (unsigned int i = 0; i < this->length; ++i) {
		this->data[i] = 0;
	}
}

MatrixPart::MatrixPart(const MatrixPart& copy) : MatrixPart(copy.length, copy.offset) {
	this->data = new Byte[this->length];
	for (unsigned int i = 0; i < this->length; ++i) {
		this->data[i] = copy[i];
	}
}

MatrixPart& MatrixPart::operator=(const MatrixPart& copy) {
	return *this;
}

MatrixPart::~MatrixPart() {
	delete this->data;
	this->data = nullptr;
}

unsigned int MatrixPart::Length() const {
	return this->length;
}

Byte& MatrixPart::operator[](const int index) const {
	return this->data[index];
}

MatrixPart& MatrixPart::operator+(const MatrixPart& copy) const {
	int offsetDiference = abs(this->offset - copy.offset);
	int length = max(this->length, copy.length) + offsetDiference;
	MatrixPart res = (length, min(this->offset, copy.offset));


}

ostream& operator<<(ostream& stream, const MatrixPart& matrix)
{
	int index = 0;
	int len = matrix.Length();
	unsigned int t;
	for (unsigned int i = 0; i < len; ++i) {
		t = matrix[len - 1 - i];
		stream << t;
	}
	return stream;
}
