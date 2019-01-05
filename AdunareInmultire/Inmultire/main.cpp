#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment (lib, "msmpi.lib")

#include "Utilities.h"

int numberOfTasks;
int currentRank;
int hostNameLength;
char hostName[MPI_MAX_PROCESSOR_NAME];
bool usingMpi = false;

void GenerateMatrix(Byte** matrix, Number x, Number y, int rowStart, int length) {
	int num;
	for (int i = rowStart; i < rowStart + length; ++i) {
		num = y[i];
		Byte* row = matrix[i];
		for (int col = 0; col < x.Length(); ++col) {
			row[col] = x[col] * y[i];
		}
	}
}

#ifdef DEBUG
void DebugPrintMatrix(Byte** matrix, int rows, int cols) {
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			cout << (int)matrix[row][cols - 1 - col] << ' ';
		}
		cout << endl;
	}
}
#endif // DEBUG



int main(int argc, char** argv) {
	int rc = MPI_Init(&argc, &argv);
	usingMpi = rc == MPI_SUCCESS;
	if (usingMpi) {
		MPI_Comm_size(MPI_COMM_WORLD, &numberOfTasks);
		MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
		MPI_Get_processor_name(hostName, &hostNameLength);
	} else {
		numberOfTasks = 1;
		currentRank = 0;
	}

	char* strX = argv[1];
	char* strY = argv[2];
	int strXLen = strlen(strX);
	int strYLen = strlen(strY);

	if (strXLen > strYLen) {
		char * t = strX;
		strX = strY;
		strY = t;
		int tl = strXLen;
		strXLen = strYLen;
		strYLen = tl;
	}

	Number x(strX);
	Number y(strY);

#ifdef DEBUG
	cout << x << endl;
	cout << y << endl;
#endif // DEBUG

	int xLen = x.Length();
	int yLen = y.Length();

	Byte** matrix = new Byte*[yLen];
	for (unsigned int i = 0; i < yLen; ++i) {
		matrix[i] = new Byte[xLen];
	}
	
	int length = y.Length() / numberOfTasks;
	int start = currentRank * length;
	GenerateMatrix(matrix, x, y, start, length); // Fac randuri in functie de rank

#ifdef DEBUG
	DebugPrintMatrix(matrix, yLen, xLen);
#endif // DEBUG

	if (currentRank != 0) {
		// Pt fiecare rank
		// Primesc ce a facut (adica randuri de matrici)

		return EXIT_SUCCESS;
	} else {
		// Trimit catre rank 0 ce am facut eu
	}

	Byte* res = new Byte[xLen + yLen];
	for (int i = 0; i < xLen + yLen; ++i) {
		res[i] = 0;
	}
	int tr = 0;
	int t;
	int sum = 0;
	int index = 0;
	for (int col = 0; col < xLen + yLen; ++col) {
		for (int row = 0; row < yLen; ++row) {
			if (col - row < 0 || col - row >= xLen) {
				continue;
			}

			t = matrix[row][col - row];
			res[col] += t;
		}

		res[col] += tr;
		tr = res[col] / 10;
		res[col] %= 10;
	}

	for (unsigned int i = 0; i < yLen; ++i) {
		delete matrix[i];
		matrix[i] = nullptr;
	}
	delete matrix;
	matrix = nullptr;

	for (int i = 0; i < xLen + yLen; ++i) {
		t = (int) res[xLen + yLen - 1 - i];
		if (!t && !i) {
			continue;
		}

		cout << t;
	}

	delete res;
	res = nullptr;

	return EXIT_SUCCESS;
}
