#include <iostream>
#include <omp.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <iterator>
#include <time.h>
#include <chrono>
using namespace std;

class Timer
{
public:
	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	void Stop()
	{
		end = std::chrono::high_resolution_clock::now();
	}
	double elapsedMilliseconds()
	{
		duration = end - start;
		return duration.count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
};

int gen()
{
	return rand() % 50;
}

float genfloat()
{
	return (rand() % 50)*0.01;
}

int findNum(int num)
{
	return num > 0 && num % 5 == 0;
}

void TASK_1() 
{
	int n = 15, limit=9;
	vector < vector<int>> D_Array(n, vector<int>(n)), New_Array(n, vector<int>(n));
	vector <int> maxNumberParallel(n), maxNumberMono(n);
	vector <string> Text(25,"Snizhana Talapchuk,v-22");
	double parallel, mono;
	Timer time;

	for(int i=0;i<n; i++)
		generate(D_Array[i].begin(), D_Array[i].end(), gen);

	cout << "Matrix:\n";
	for (int i = 0; i < n; i++) {
		copy(D_Array[i].begin(), D_Array[i].end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}

    #pragma omp parallel num_threads(n) shared(D_Array, New_Array,maxNumberParallel)
	{
		#pragma omp master
		{
			cout << "\nNumber Theads: " << omp_get_thread_num()<< " : " <<*Text.data()<<"\n";
		} 
	}

	cout << "\n\n Parallel:\n";
	time.Start();
	#pragma omp parallel num_threads(n) shared(D_Array,New_Array,maxNumberParallel)
	{  
		#pragma omp for schedule(static,8)
		//#pragma omp for schedule(guided,8)
			for(int i=0; i< n; i++)
			{
				for (int j = 0; j < n; j++)
					if (D_Array[i][j] < limit) New_Array[i][j] = D_Array[i][j];
				maxNumberParallel[i]=*max_element(New_Array[i].begin(), New_Array[i].end());
				cout << "\nNumber Theads: " << omp_get_thread_num() << " the maximum element in the row: " << i << " - " << maxNumberParallel[i] << "\n";
			}
	}
	time.Stop();
	parallel = time.elapsedMilliseconds();
	cout << "\nParallel time: " << parallel<<"\n";

	cout << "\n\n Mono-thread:\n";
	time.Start();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			if (D_Array[i][j] < limit) New_Array[i][j] = D_Array[i][j];
		maxNumberMono[i] = *max_element(New_Array[i].begin(), New_Array[i].end());
		cout << "\nNumber Theads: " << omp_get_thread_num() << " the maximum element in the row: " << i << " - " << maxNumberMono[i] << "\n";
	}
	time.Stop();
	mono= time.elapsedMilliseconds();
	cout << "\nMono-thread time: " << mono<<"\n";

	if (mono > parallel)
		cout << "\nParallel thread faster by: " << mono - parallel<<" seconds \n";
	else cout << "\nMono-thread faster by: " << parallel - mono << " seconds \n";

	if(maxNumberParallel== maxNumberMono)
		cout << "\nmaxNumberParallel and maxNumberMono are equal" << endl;
	else  cout << "\nmaxNumberParallel and maxNumberMono are not equal" << endl;
}

void TASK_2()
{
#pragma omp parallel
	{
	#pragma omp sections
		{
		#pragma omp section
			{ 
			vector<int> ArrayFSection(77);
			generate(ArrayFSection.begin(), ArrayFSection.end(), gen);

			cout << "\nArrayFSection:\n";
			copy(ArrayFSection.begin(), ArrayFSection.end(), ostream_iterator<int>(cout, " "));
			cout << endl;

			auto result1 = find_if(ArrayFSection.begin(), ArrayFSection.end(), findNum);
			if (result1!=ArrayFSection.end())
				cout << "\n\nFound positive numbers of multiple 5\n";
			    else cout << "\n\nElement not found\n";
			}

		#pragma omp section
			{  int size = 4;
				vector<vector<float>> ArraySSection(size, vector<float>(size));

				for(int i=0;i<size;i++)
					generate(ArraySSection[i].begin(), ArraySSection[i].end(), genfloat);

				cout << "\nArraySSection:\n";
				for (int i = 0; i < size; i++)
				{
					copy(ArraySSection[i].begin(), ArraySSection[i].end(), ostream_iterator<float>(cout, " "));
					cout << endl;
				}

				cout << "\nArraySSection sorters:\n";
				for(int i=0; i < size; i++)
				{
					sort(ArraySSection[i].begin(), ArraySSection[i].end(), greater<float>());
					copy(ArraySSection[i].begin(), ArraySSection[i].end(), ostream_iterator<float>(cout, " "));
					cout << endl;
				}
			}
		}
	}
}

int main()
{
	TASK_1();
	TASK_2();
	system("pause");
	return 0;
}