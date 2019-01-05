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


