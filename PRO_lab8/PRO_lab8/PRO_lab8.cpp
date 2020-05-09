#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/tick_count.h>
#include <iostream>
#include <string>
#include <time.h>
#include <chrono>
#include <math.h>
using namespace tbb;
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
	std::chrono::duration<float, milli> duration;
};

struct Point {
	double x, y;
};
class turnCounter
{
public:
	turnCounter(Point points[], Point pointsTurn[], double radian) :points_(points), pointsTurn_(pointsTurn), radian_(radian) {}
	Point* operator()(const blocked_range<size_t > &r)const
	{
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			pointsTurn_[i].x = points_[i].x*cos(radian_) - points_[i].y*sin(radian_);
			pointsTurn_[i].y = points_[i].x*sin(radian_) + points_[i].y*cos(radian_);
		}
		return pointsTurn_;
	}
private:
	Point *const points_;
	Point *const pointsTurn_;
	double const radian_;
};
Point* turnCounterclockwise(Point points[], Point pointsTurn1[], size_t num, double radian)
{
	for (size_t i = 0; i < num; i++)
	{
		pointsTurn1[i].x = points[i].x*cos(radian) - points[i].y*sin(radian);
		pointsTurn1[i].y = points[i].x*sin(radian) + points[i].y*cos(radian);
	}

	return pointsTurn1;
}
int main()
{
	Point points[3], pointsTurn[3], pointsTurn1[3];
	double angle, pi = 3.14159265358979323846, radian;
	Timer time;
	size_t num = 3;
	cout << "Enter triangle vertices:\n";
	for (size_t i = 0; i < num; ++i) {
		cout << "X" << i + 1 << " : ";
		cin >> points[i].x;
		cout << "Y" << i + 1 << " : ";
		cin >> points[i].y;
	}

	cout << "Enter the angle of rotation of the triangle(degrees):\n";
	cin >> angle;
	radian = (angle * (pi / 180));
	time.Start();
	turnCounterclockwise(points, pointsTurn1, num, radian);
	time.Stop();
	cout << "\nMono-thread time: " << time.elapsedMilliseconds() << endl;
	time.Start();
	task_scheduler_init init(3);
	parallel_for(blocked_range<size_t>(0, num), turnCounter(points, pointsTurn, radian), auto_partitioner());
	time.Stop();
	cout << "Parallel time: " << time.elapsedMilliseconds() << endl;
	cout << "New triangle vertices: " << endl;
	for (size_t i = 0; i < num; i++)
	{
		cout << "X" << i + 1 << " : " << pointsTurn[i].x << endl << "Y" << i + 1 << " : " << pointsTurn[i].y << endl;
	}
}