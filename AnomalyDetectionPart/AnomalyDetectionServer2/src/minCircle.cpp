// 31140826

/*
 * minCircle.cpp
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */

#include "minCircle.h"
#include <iostream>
#include <cmath>
#include <vector>

// This function returns distance between two points.
float dist(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// This function form a circle that intersect 3 points.
Circle from3Points(Point a, Point b, Point c) {

    // Calculate the center of a circle defined by 3 points.
    float delta1x = b.x - a.x;
    float delta1y = b.y - a.y;
    float delta2x = c.x - a.x;
    float delta2y = c.y - a.y;
    float phrase1 = delta1x * delta1x + delta1y * delta1y;
    float phrase2 = delta2x * delta2x + delta2y * delta2y;
    float phrase3 = delta1x * delta2y - delta1y * delta2x;
    float px = (delta2y * phrase1 - delta1y * phrase2) / (2 * phrase3);
    float py = (delta1x * phrase2 - delta2x * phrase1) / (2 * phrase3);
    Point center = Point(px, py);

    // Create a circle that intersect the three points, and return it.
    center.x += a.x;
    center.y += a.y;
    return {center, dist(center, a)};

}

// This function form a circle that intersect 2 points.
Circle from2points(Point a, Point b) {

    // Calculate the center between the two given points.
    float mx = (a.x + b.x) / 2;
    float my = (a.y + b.y) / 2;
    Point center = Point(mx, my);

    // The radius is the diameter divide by 2.
    float radius = dist(a, b) / 2;

    // Return the only possible circle for this case.
    return {center, radius};

}


// This function checks if a circle encloses given points.
bool isValid(const Circle& circle, const vector<Point>& points) {

    // If there's a point outside the circle, than it isn't a valid circle.
    for (const Point& p : points) {
        if (dist(circle.center, p) > circle.radius) {
            return false;
        }
    }

    // If all the points are inside the circle, than it is valid.
    return true;

}

// This function returns the minimum enclosing circle for max 3 points.
Circle trivial(vector<Point>& points) {

    // Split to cases according to number of points.

    // If there are no points, return "empty circle".
    if (points.empty()) {
        Point center = Point(0, 0);
        return {center, 0};;
    }

    // If there is one point, than it's the center of a circle with radius 0.
    else if (points.size() == 1) {
        return {points[0], 0};
    }

    // If there are two points, use them to create a circle.
    else if (points.size() == 2) {
        return from2points(points[0], points[1]);
    }

    // If there are three, check for a VALID circle that formed from 2 points.
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle circle = from2points(points[i], points[j]);
            if (isValid(circle, points)) {
                return circle;
            }
        }
    }

    // If there's no valid circle from 2 points, than return a 3-points circle.
    return from3Points(points[0], points[1], points[2]);

}

// Using Welzl's algorithm from the literature (Wikipedia).
Circle welzl(Point** points, vector<Point> boundary, size_t size) {

    // If points is "empty" or 3 points are on the boundary, return trivial.
    if (size == 0 || boundary.size() == 3) {
        return trivial(boundary);
    }

    // Choose random point from points.
    size_t i = rand() % size;
    float px = points[i]->x;
    float py = points[i]->y;
    Point p = Point(px, py);

    // Put random point in the end to ignore it (instead of removing it).
    swap(points[i], points[size - 1]);

    // Recursively get a MEC and if is contains point p, return it.
    Circle circle = welzl(points, boundary, size - 1);
    if (dist(circle.center, p) < circle.radius) {
        return circle;
    }

    // Else, push p to the boundary set (vector) and continue with recursion.
    boundary.push_back(p);
    return welzl(points, boundary, size - 1);

}

// Entry point of the algorithm.
Circle findMinCircle(Point** points, size_t size){
    return welzl(points, {}, size);
}