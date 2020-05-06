#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h> 
#include <iostream>
#include <string>
#include <time.h>
#include <chrono>
#include <math.h>
#include <cilk/reducer.h>
#include <cilk/cilk_api.h>
using namespace std;
cilk::reducer_opadd<double> radian(0);
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
	std::chrono::duration<float, milli> duration;
};

struct Point {
	double x, y;
};

Point* turnCounterclockwise(Point points[], Point pointsTurn[], int num)
{

	for (int i = 0; i < num; i++)
	{
		pointsTurn[i].x = points[i].x*cos(radian.get_value()) - points[i].y*sin(radian.get_value());
		pointsTurn[i].y= points[i].x*sin(radian.get_value()) + points[i].y*cos(radian.get_value());
	}

	return pointsTurn;
}
Point* turnCounterclockwiseCilk(Point points[], Point pointsTurn[], int num)
{
	__cilkrts_set_param("nworkers", "3");
	cilk_for (int i = 0; i < num; i++)
	{
		pointsTurn[i].x = points[i].x*cos(radian.get_value()) - points[i].y*sin(radian.get_value());
		pointsTurn[i].y = points[i].x*sin(radian.get_value()) + points[i].y*cos(radian.get_value());
	}

	return pointsTurn;
}

int main()
{
	Point points[3], pointsTurn[3], pointsTurn1[3];
	double angle, pi= 3.14159265358979323846;
	Timer time;
	int num = 3;
	cout << "Enter triangle vertices:\n";
	for (int i = 0; i < num; ++i) {
		cout << "X" << i + 1 << " : ";
		cin >> points[i].x;
		cout << "Y" << i + 1 << " : ";
		cin >> points[i].y;
	}
	
	cout << "Enter the angle of rotation of the triangle(degrees):\n";
	cin >> angle;
	radian+= (angle * (pi / 180));
	time.Start();
	turnCounterclockwise(points,pointsTurn1,num);
	time.Stop();
	cout << "\nMono-thread time: " << time.elapsedMilliseconds() <<endl;
	time.Start();
	turnCounterclockwiseCilk(points,pointsTurn, num);
	time.Stop();
	cout << "Parallel time: " << time.elapsedMilliseconds() <<endl;
	cout << "New triangle vertices: " << endl;
	for (int i = 0; i < num; i++)
	{
		cout << "X" << i + 1 << " : " << pointsTurn[i].x <<endl<< "Y" << i + 1 << " : " << pointsTurn[i].y << endl;
	}

}