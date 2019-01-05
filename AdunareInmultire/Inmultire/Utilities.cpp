#include "Utilities.h"

Number::Number(const char* const str): length(strlen(str) / 2) {
	this->data = new Byte[this->length];

	for (unsigned int i = 0; i < this->length * 2; ++i) {
		this->Set(i, str[i] - '0');
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

unsigned int Number::Length() {
	return this->length;
}

ostream& operator<<(ostream& stream, const Number& number)
{
	for (unsigned int i = 0; i < number.length * 2; ++i) {
		stream << number[i];
	}
	return stream;
}
