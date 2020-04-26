#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <omp.h>
using namespace std;


void TASK()
{
	int m = 5, n = 8;
	vector<vector<int>> Matrix(m, vector<int>(n));
	
	for (int i = 0; i < Matrix.size(); i++) {
		generate(Matrix[i].begin(), Matrix[i].end(), []() { return rand() % 100; });
		copy(Matrix[i].begin(), Matrix[i].end(), ostream_iterator<int>(cout, "\t"));
		cout << endl;
	}

#pragma omp parallel num_threads(m) shared(Matrix) 
	{
#pragma omp for
		for (int i = 0; i < m; i++)
		{
#pragma omp critical 
			{
				cout<<"\nNumber Theads: " << omp_get_thread_num() << " the minimum element in the row: " << i << " - " << *min_element(Matrix[i].begin(), Matrix[i].end());
			}
		}
	}
	cout << endl;
}

int main()
{
	srand(time(NULL));
	TASK();
	system("pause");
	return 0;
}
