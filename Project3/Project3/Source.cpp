#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
using namespace std;
MPI_Status status;
MPI_Request request;
int arr[36];
int msg[4];
int pro[9] = {0,1,2,3,4,5,6,7,8};
int a[36];
int bf[8];
int *buf;
int t = 30;
void bubblesort(int *a, int n)
{
	int i, j;
	for (i = n - 1; i >= 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);
		}
	}
}
void p(int rank, int p1, int p2)
{
	if (rank == p2)
	{
		buf = new int[t + MPI_BSEND_OVERHEAD];
		MPI_Buffer_attach(buf, t + MPI_BSEND_OVERHEAD);
			MPI_Ibsend(msg, 4, MPI_INT, p1, 0, MPI_COMM_WORLD,
				&request);
		MPI_Buffer_detach(&buf, &t);
		MPI_Wait(&request, &status);
		MPI_Irecv(msg, 4, MPI_INT, p1, 0, MPI_COMM_WORLD,
			&request);
		MPI_Wait(&request, &status);
	}
	if (rank == p1)
	{
		MPI_Irecv(bf, 4, MPI_INT, p2, 0, MPI_COMM_WORLD,
			&request);
		MPI_Wait(&request, &status);
		for (int i = 4; i < 8; i++)
		{
			bf[i] = msg[i - 4];
		}
		bubblesort(bf, 8);
		for (int i = 0; i < 4; i++)
		{
			msg[i] = bf[i];
		}
		buf = new int[t + MPI_BSEND_OVERHEAD];
		MPI_Buffer_attach(buf, t + MPI_BSEND_OVERHEAD);
		MPI_Ibsend(bf + 4, 4, MPI_INT, p2, 0, MPI_COMM_WORLD,
			&request);
		MPI_Buffer_detach(&buf, &t);
		MPI_Wait(&request, &status);
	}
}
int main(int argc, char *argv[])
{
	int rank;
	int size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//task2
	for (int i = 0; i < 36; i++)
	{
		arr[i] = rand() % 100;
	}
	if (size == 9)
	{
		MPI_Scatter(arr, 4, MPI_INT, msg, 4, MPI_INT, 0, MPI_COMM_WORLD);
		p(rank, 0, 1);
	    p(rank, 0, 2);
		p(rank, 0, 3);
		p(rank, 0, 4);
		p(rank, 0, 5);
		p(rank, 0, 6);
		p(rank, 0, 7);
		p(rank, 0, 8);
		p(rank, 1, 2);
		p(rank, 1, 3);
		p(rank, 1, 4);
		p(rank, 1, 5);
		p(rank, 1, 6);
		p(rank, 1, 7);
		p(rank, 1, 8);
		p(rank, 2, 3);
		p(rank, 2, 4);
		p(rank, 2, 5);
		p(rank, 2, 6);
		p(rank, 2, 7);
		p(rank, 2, 8);
		p(rank, 3, 4);
		p(rank, 3, 5);
		p(rank, 3, 6);
		p(rank, 3, 7);
		p(rank, 3, 8);
		p(rank, 4, 5);
		p(rank, 4, 6);
		p(rank, 4, 7);
		p(rank, 4, 8);
		p(rank, 5, 6);
		p(rank, 5, 7);
		p(rank, 5, 8);
		p(rank, 6, 7);
		p(rank, 6, 8);
		p(rank, 7, 8);
		MPI_Gather(msg, 4, MPI_INT, a, 4, MPI_INT, 0, MPI_COMM_WORLD);
		if (rank == 0)
		{
			for (int i = 0; i < 36; i++)
			{
				cout << arr[i] << " ";
			}
			cout << endl;
			for (int i = 0; i < 36; i++)
			{
				cout << a[i] << " ";
			}
			cout << "\n ";
		}
	}
	//task1
	for (int i = 0; i < 9; i++)
	{
		MPI_Bcast(&pro[i], 1, MPI_INT, i, MPI_COMM_WORLD);
	}
	if(rank!=1 && rank!=7)
	{
		for(int i=0; i<9; i++)
		{
			cout << "\nFrom rank #" << rank << ": The received message: " << pro[i];
		}
	}
	MPI_Finalize();
	return 0;
}