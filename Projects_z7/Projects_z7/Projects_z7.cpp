#include <cilk/cilk.h>
#include <iostream>
#include <ctime>
# include <math.h>
#include <string>
#include <vector>
using namespace std;


vector < vector <int> > Multiply(vector < vector <int> >& a, vector < vector <int> >& b, vector < vector <int> >& c, int n)
{
	cilk_for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++) {
			c[i][j] = 0;
			for (int k = 0; k < n; k++)
				c[i][j]+= a[i][k] * b[k][j];
		}
	}
	return c;
}
int pred(int num) { return rand() % 20 + num; }
vector < vector <int> > MatrixRandom(vector < vector <int> >& a, int num)
{
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < a.size(); j++)
		a[i][j]=pred(num);
	}
	return a;
}
void OutputMatrix(vector < vector <int> >& a) {
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < a.size(); j++)
			cout << a[i][j] << "\t";
		cout << endl;
	}
	cout << endl;
}
int main()
{
	int n;
	cout << "Enter the size of the matrices n = " <<endl;
	cin >> n;
	vector < vector <int> > A(n, vector<int>(n)),B(n, vector<int>(n)),C(n, vector<int>(n));
	cilk_spawn MatrixRandom(A, 10);
	MatrixRandom(B, 3);
	cilk_sync;
	cout << "Matrix A:" <<endl;
	OutputMatrix(A);
	cout << "Matrix B:" << endl;
	OutputMatrix(B);
	Multiply(A, B, C, n);
	cout << "Matrix C:" << endl;
	OutputMatrix(C);

}