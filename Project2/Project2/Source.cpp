#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <ctime> 
#include <cmath>
using namespace std;
int main(int argc, char** argv)
{
	int myrank, size, number = 8;
	MPI_Status status;
	char* buffer;
	int buffsize = 45;
	int TAG = 0;
	char message[24];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (myrank == 0)
	{
		MPI_Send(&number, 10, MPI_INT, 1, TAG, MPI_COMM_WORLD);// стандартний
		MPI_Recv(&number, 10, MPI_INT, 3, TAG, MPI_COMM_WORLD, &status);//стандартний
		if (number < 569)
		{
			number *= 8;
			MPI_Send(&number, 10, MPI_INT, 1, TAG, MPI_COMM_WORLD);// стандартний
		}
		else
		cout << "Number-0: " << number << endl;

		buffer = (char*)malloc(buffsize + MPI_BSEND_OVERHEAD);
		MPI_Buffer_attach(buffer, buffsize + MPI_BSEND_OVERHEAD);
		buffer = (char*)"Everyone is the creator of one's own fate.";
		MPI_Bsend(&buffer, buffsize, MPI_CHAR, 1, TAG, MPI_COMM_WORLD);//буферний
		MPI_Buffer_detach(&buffer, &buffsize);
		MPI_Recv(&buffer, buffsize, MPI_CHAR, 3, TAG, MPI_COMM_WORLD, &status);//буферний
		cout << "received0: " << buffer << endl;

		strcpy(message, "Synchronous exchange");
		for (int i = 1; i < size; i += 2)
		{
			MPI_Ssend(&message, 25, MPI_CHAR, i, TAG, MPI_COMM_WORLD);//синхронний
		}
		MPI_Recv(&message, 25, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, &status);//синхронний
		cout << "Rank0-1" << " Message= " << message << endl;
		MPI_Recv(&message, 25, MPI_CHAR, 3, TAG, MPI_COMM_WORLD, &status);//синхронний
		cout << "Rank0-3" << " Message= " << message << endl;

	}
	else if (myrank == 1)
	{
		MPI_Recv(&number, 10, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);//стандартний
		if (number < 569) 
		{
			number *=8;
			MPI_Send(&number, 10, MPI_INT, 2, TAG, MPI_COMM_WORLD);// стандартний
		}
		else
		cout << "Number-1: " << number << endl;


		buffer = (char*)malloc(buffsize + MPI_BSEND_OVERHEAD);
		MPI_Buffer_attach(buffer, buffsize + MPI_BSEND_OVERHEAD);
		MPI_Recv(&buffer, buffsize, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status);//буферний
		cout << "received1: " << buffer << endl;
		MPI_Bsend(&buffer, buffsize, MPI_CHAR, 2, TAG, MPI_COMM_WORLD);//буферний
		MPI_Buffer_detach(&buffer, &buffsize);

		MPI_Recv(&message, 25, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status);//синхронний
		cout << "Rank1-0" << " Message= " << message << endl;
		for (int i = 0; i < size; i += 2)
		{
			MPI_Ssend(&message, 25, MPI_CHAR, i, TAG, MPI_COMM_WORLD);//синхронний
		}
	}
	else if (myrank == 2)
	{
		MPI_Recv(&number, 10, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);//стандартний
		if (number < 569)
		{
			number *=8;
			MPI_Send(&number, 10, MPI_INT, 3, TAG, MPI_COMM_WORLD);// стандартний
		}
		else
			cout << "Number-2: " << number << endl;


		buffer = (char*)malloc(buffsize + MPI_BSEND_OVERHEAD);
		MPI_Buffer_attach(buffer, buffsize + MPI_BSEND_OVERHEAD);
		MPI_Recv(&buffer, buffsize, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, &status);//буферний
		cout << "received2: " << buffer << endl;
		MPI_Bsend(&buffer, buffsize, MPI_CHAR, 3, TAG, MPI_COMM_WORLD);//буферний
		MPI_Buffer_detach(&buffer, &buffsize);

		MPI_Recv(&message, 25, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, &status);//синхронний
		cout << "Rank2-1" << " Message= " << message << endl;
		MPI_Recv(&message, 25, MPI_CHAR, 3, TAG, MPI_COMM_WORLD, &status);//синхронний
		cout << "Rank2-3" << " Message= " << message << endl;
	}
	else if (myrank == 3)
	{
		MPI_Recv(&number, 10, MPI_INT, 2, TAG, MPI_COMM_WORLD, &status);//стандартний
		if (number < 569)
		{
			number *=8;
			MPI_Send(&number, 10, MPI_INT, 0, TAG, MPI_COMM_WORLD);// стандартний
		}
		else
		cout << "Number-3: " << number << endl;
		


		buffer = (char*)malloc(buffsize + MPI_BSEND_OVERHEAD);
		MPI_Buffer_attach(buffer, buffsize + MPI_BSEND_OVERHEAD);
		MPI_Recv(&buffer, buffsize, MPI_CHAR, 2, TAG, MPI_COMM_WORLD, &status);//буферний
		cout << "received3: " << buffer << endl;
		MPI_Bsend(&buffer, buffsize, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);//буферний
		MPI_Buffer_detach(&buffer, &buffsize);

		MPI_Recv(&message, 25, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status);//синхронний
		cout << "Rank3-0" << " Message= " << message << endl;
		for (int i = 0; i < size; i += 2)
		{
			MPI_Ssend(&message, 25, MPI_CHAR, i, TAG, MPI_COMM_WORLD);//синхронний
		}
	}

	MPI_Finalize(); 
	return 0;
}