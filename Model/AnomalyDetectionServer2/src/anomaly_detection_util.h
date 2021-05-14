/*
 * anomaly_detection_util.h
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */

#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

// Returns the average of the floats in array x.
float avg(float* x, int size);

// Returns the variance of x and x.
float var(float* x, int size);

// Returns the covariance of x and y.
float cov(float* x,  float* y, int size);

// Returns the Pearson correlation coefficient of x and y.
float pearson(float* x, float* y, int size);

// Defines line.
class Line{
public:
	float a,b;
	Line():a(0),b(0){};
	Line(float a, float b):a(a),b(b){}
	float f(float x){
		return a*x+b;
	}
};

// Defines point.
class Point{
public:
	float x,y;
	Point(float x, float y):x(x),y(y){}
    Point():x(0),y(0){}
};

// Performs a linear regression and returns the line equation.
Line linear_reg(Point** points, int size);

// Returns the deviation between point p and the line equation of the points.
float dev(Point p,Point** points, int size);

// Returns the deviation between point p and the line.
float dev(Point p,Line l);

#endif
