#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <ctime> 
#include "Header.h"

using namespace std;

int* getRandomArray(int size);

int main(int argc, char** argv)
{

    int rank, root_process, ierr, r_size;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &r_size);

    if (rank == 0) {
        firstTask();
    }
    else if (rank == 1) {
        secondTask();
    }
    else {
        cout << "No task for the process - " << rank  << endl ; 
    }
    MPI_Finalize();
    return 0;
}

void secondTask()
{
    cout << "Task 2 " << endl;
    float array[4][4];
    srand((unsigned)time(0));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            array[i][j] = (rand() % 100) + 1;
            cout << array[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    float tmp = 0;
    for (int i = 0; i < 4; i++) {
        for (int k = 1; k < 4; k++) {
            for (int j = 3; j >= k; j--) {
                if (array[i][j] > array[i][j -1]) {
                    tmp = array[i][j - 1];
                    array[i][j - 1] = array[i][j];
                    array[i][j] = tmp;

                }
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << array[i][j] << " ";
        }
        cout << endl;
    }
   
}

void firstTask()
{
    cout << "Task 1 " << endl;
    int size = 77;
    int* array = getRandomArray(size);
    int count = 0;
    for (int i = 0; i < size; i++) {
        if ((array[i] % 5 == 0) && (array[i] > 0)) {
            count += 1;
        }
    }
    if (count > 0) {
        cout << "Task 1,  have -  " << count<< " positive numbers multiples 5" << endl;
    }
   
}
int* getRandomArray(int size) {
    int* array = new int[size];
    srand((unsigned)time(0));
    for (int i = 0; i < size; i++) {
        array[i] = (rand() % 100) + 1;
        cout << array[i] << " ";
    }
    cout << endl;
    return array;
}