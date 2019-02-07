#include "Utilities.h"

int Abs(int x) {
	if (x < 0) {
		return -x;
	}
	return x;
}

int Min(int x, int y) {
	return x < y ? x : y;
}

int Max(int x, int y) {
	return x > y ? x : y;
}


Number::Number(const char* const string) {
	int stringLength = strlen(string);
	this->length = stringLength / 2;
	if (stringLength % 2) {
		this->length = (stringLength + 1) / 2;
	}
	this->data = new Byte[this->length];
	for (unsigned int i = 0; i < this->length; ++i) {
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

ostream& operator<<(ostream& stream, const Number& number) {
	int index = 0;
	unsigned int len = number.Length();
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

MatrixPart::MatrixPart(const Byte* const number, const unsigned int length, const unsigned int offset) : MatrixPart(length, offset) {
	this->data = new Byte[this->length];
	for (unsigned int i = 0; i < this->length; ++i) {
		this->data[i] = number[i];
	}
}

MatrixPart::MatrixPart(const MatrixPart& copy) : MatrixPart(copy.length, copy.offset) {
	this->data = new Byte[this->length];
	for (unsigned int i = 0; i < this->length; ++i) {
		this->data[i] = copy[i];
	}
}

MatrixPart& MatrixPart::operator=(const MatrixPart& copy) {
	this->length = copy.length; 
	this->offset = copy.offset;

	this->data = new Byte[this->length];
	for (unsigned int i = 0; i < this->length; ++i) {
		this->data[i] = copy[i];
	}

	return *this;
}

MatrixPart::~MatrixPart() {
	delete this->data;
	this->data = nullptr;
}

unsigned int MatrixPart::Length() const {
	return this->length;
}

unsigned int MatrixPart::GetOffset() const {
	return this->offset;
}

Byte*& MatrixPart::operator*() {
	return this->data;
}

Byte& MatrixPart::operator[](const int index) const {
	return this->data[index];
}

MatrixPart MatrixPart::operator+(const MatrixPart& other) const {
	int offsetDiference = Abs(this->offset - other.offset);
	int length = Max(this->length, other.length) + offsetDiference + 1;
	int offset = Min(this->offset, other.offset);

	const MatrixPart* firstPointer = this;
	const MatrixPart* secondPointer = &other;

	if (other.offset < this->offset) {
		secondPointer = this;
		firstPointer = &other;
	}

	const MatrixPart& first = *firstPointer;
	const MatrixPart& second = *secondPointer;

	MatrixPart res = MatrixPart(length, offset);
	Byte transport = 0;
	for (unsigned int i = 0; i < length; ++i) {
		if (i < offsetDiference) {
			res[i] = first[i];
			continue;
		}
		if (i < first.Length() && i - offsetDiference < second.Length()) {
			res[i] = first[i] + second[i - offsetDiference];
			res[i] += transport;
			transport = res[i] / 10;
			res[i] %= 10;
			continue;
		}
		if (i - offsetDiference < second.Length()) {
			res[i] = second[i - offsetDiference];
			res[i] += transport;
			transport = res[i] / 10;
			res[i] %= 10;
			continue;
		}
		res[i] = transport;
		transport = res[i] / 10;
		res[i] %= 10;
	}

	return res.Reduce();
}

MatrixPart& MatrixPart::operator+=(const MatrixPart& other) {
	*this = *this + other;
	return *this;
}

MatrixPart& MatrixPart::Reduce() {
	unsigned int lastZeroes = 0;
	for (unsigned int i = this->length - 1; i >= 0 && !this->data[i]; --i) {
		++lastZeroes;
	}
	this->length -= lastZeroes;
	if (this->length < 0) {
		this->length = 0;
	}
	Byte* newData = new Byte[this->length];
	for (unsigned int i = 0; i < this->length; ++i) {
		newData[i] = this->data[i];
	}

	delete this->data;
	this->data = newData;

	return *this;
}

void MatrixPart::Output(ostream& stream) const {
	int index = 0;
	unsigned int len = this->Length();
	unsigned int t;
	for (unsigned int i = 0; i < len; ++i) {
		t = (*this)[len - 1 - i];
		stream << t;
	}
	for (unsigned int i = 0; i < this->offset; ++i) {
		stream << '0';
	}
	stream << endl;
}

ostream& operator<<(ostream& stream, const MatrixPart& matrix) {
	int index = 0;
	unsigned int len = matrix.Length();
	unsigned int t;
	for (unsigned int i = 0; i < len; ++i) {
		t = matrix[len - 1 - i];
		stream << t << ' ';
	}
	for (unsigned int i = 0; i < matrix.offset; ++i) {
		stream << "0 ";
	}
	return stream;
}
