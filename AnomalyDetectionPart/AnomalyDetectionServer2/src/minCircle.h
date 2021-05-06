/*
 * minCircle.h
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

// This function returns distance between two points.
float dist(Point a, Point b);

// This function form a circle that intersect 2 points.
Circle from2points(Point a,Point b);

// This function form a circle that intersect 3 points.
Circle from3Points(Point a, Point b, Point c);

// This function returns the minimum enclosing circle for max 3 points.
Circle trivial(vector<Point>& P);

// Welzl's algorithm to find MEC.
Circle welzl(Point** P,vector<Point> R, size_t n);

// Entry point of the algorithm.
Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */
