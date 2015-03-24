#ifndef POINT
#define POINT
class Point{
public:
	Point(){ from = NULL; }
	int x;
	int y;
	Point* from;

	int distanceToGoal;
};

#endif