#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment (lib, "msmpi.lib")

#include "Utilities.h"

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
	cout << x << endl;
	cout << y << endl;

	Byte** matrix = new Byte*[y.Length()];
	for (unsigned int i = 0; i < y.Length(); ++i) {
		matrix[i] = new Byte[y.Length() + x.Length()];
	}

	int length = y.Length() / numberOfTasks;
	int start = currentRank * length;
	for (int i = start; i < start + length; ++i) {
		int num = y[start + length - 1 - i];

	}

	return EXIT_SUCCESS;
}
