#pragma once

#include <conio.h>

#define DEBUG(M) cout << M
#undef DEBUG

#ifdef DEBUG
void DebugPrintMatrix(Byte** matrix, int rows, int cols) {
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			cout << (int)matrix[row][cols - 1 - col] << ' ';
		}
		cout << endl;
	}
}

void DebugNumber(Byte* number, int len) {
	int index = 0;
	int t;
	for (; index < len; ++index) {
		t = number[len - 1 - index];
		if (t != 0) {
			break;
		}
	}

	if (index == len) {
		cout << '0';
		return;
	}

	for (; index < len; ++index) {
		t = (int) number[len - 1 - index];
		cout << t;
	}
}

#define DEBUG_NUMBER(N, L) DebugNumber(N, L)

#define DEBUG_MATRIX(M, R, C) DebugPrintMatrix(M, R, C)

#define DEBUG_WAIT cout << "Press a key..."; _getch()

#else

#define DEBUG_NUMBER(N, L)
#define DEBUG_MATRIX(M, R, C)
#define DEBUG_WAIT

#define DEBUG(M)

#endif // DEBUG


