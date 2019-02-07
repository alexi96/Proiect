#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment (lib, "msmpi.lib")

#include "Utilities.h"

#include "Debug.h"

int numberOfTasks;
int currentRank;
int hostNameLength;
char hostName[MPI_MAX_PROCESSOR_NAME];
bool usingMpi = false;

unsigned int LengthPerTask(unsigned int totalLength) {
	unsigned int res = totalLength / numberOfTasks;
	if (currentRank == 0 && totalLength % numberOfTasks != 0) {
		res += totalLength % numberOfTasks;
	}
	return res;
}

unsigned int StartRow(unsigned int totalLength) {
	unsigned int res = totalLength / numberOfTasks * currentRank;
	if (currentRank != 0 && totalLength % numberOfTasks != 0) {
		res += totalLength % numberOfTasks;
	}
	return res;
}

MatrixPart GenerateMatrixPart(Number& x, Number& y) {
	unsigned int yLen = y.Length();
	unsigned int xLen = x.Length();
	unsigned int rows = LengthPerTask(y.Length());
	unsigned int offset = StartRow(y.Length());
	unsigned int startingRow = offset;

	MatrixPart res(xLen, startingRow);
	
	Byte* data = new Byte[xLen];
	for (unsigned int row = startingRow; row < startingRow + rows; ++row) {
		for (unsigned int col = 0; col < xLen; ++col) {
			data[col] = x[col] * y[row];
		}
		MatrixPart t(data, xLen, row);
		res += t;

		DEBUG("Matrix part rank: ");
		DEBUG(currentRank);
		DEBUG(" : ");
		DEBUG(res);
		DEBUG("\n");
	}

	return res;
}

int main(int argc, char** argv) {
	int rc = MPI_Init(&argc, &argv);
	usingMpi = rc == MPI_SUCCESS;
	if (usingMpi) {
		MPI_Comm_size(MPI_COMM_WORLD, &numberOfTasks);
		MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
		MPI_Get_processor_name(hostName, &hostNameLength);
	}

	if (argc == 0) {
		cerr << "Need 2 arguments";
		return EXIT_FAILURE;
	}

	char* strX = argv[1];
	char* strY = argv[2];
	int strXLen = strlen(strX);
	int strYLen = strlen(strY);

	if (strXLen < strYLen) {
		char * t = strX;
		strX = strY;
		strY = t;
		int tl = strXLen;
		strXLen = strYLen;
		strYLen = tl;
	}

	Number x(strX);
	Number y(strY);

	DEBUG("X: ");
	DEBUG(x);
	DEBUG("\n");
	DEBUG("Y: ");
	DEBUG(y);
	DEBUG("\n");

	int xLen = x.Length();
	int yLen = y.Length();
	int lengthPerTask = yLen / numberOfTasks;
	
	if (lengthPerTask == 0) { // Daca numarul e prea mic
		if (currentRank == 0) { // il face procesul cu rank 0 pe tot
			lengthPerTask = y.Length();
			numberOfTasks = 1;
		} else { // si altii se opresc
			MPI_Finalize();
			return EXIT_SUCCESS;
		}
	}

	lengthPerTask = LengthPerTask(yLen);

	DEBUG("Rows by rank ");
	DEBUG(currentRank);
	DEBUG(" : ");
	DEBUG(lengthPerTask);
	DEBUG("\n");

	MatrixPart matrixPart = GenerateMatrixPart(x, y);
	
	if (!usingMpi || currentRank == 0) {
		// Pt fiecare rank
		int otherRankRowStart;
		for (int otherRank = 1; otherRank < numberOfTasks; ++otherRank) {
			// Primesc ce a facut (adica randuri de matrici)
			otherRankRowStart = otherRank * lengthPerTask;

			int len;
			MPI_Recv(&len, 1, MPI_INT, otherRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
			MatrixPart t(len, otherRankRowStart);
			MPI_Recv(*t, t.Length(), MPI_BYTE, otherRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);			
			matrixPart += t;
			DEBUG(t);
			DEBUG('\n');
			DEBUG(matrixPart);
			DEBUG('\n');
		}
	} else if (usingMpi) {
		// Trimit catre rank 0 ce am facut eu
		int len = matrixPart.Length();
		MPI_Send(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(*matrixPart, len, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

		MPI_Finalize();
		return EXIT_SUCCESS;
	}

	MPI_Finalize();

	matrixPart.Output(cout);

#ifdef DEBUG
	//_getch();
#endif // DEBUG

	return EXIT_SUCCESS;
}
