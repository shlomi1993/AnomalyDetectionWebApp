/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */
#include "HybridAnomalyDetector.h"

// Constructor (empty) implementation - relying on parent's constructor.
HybridAnomalyDetector::HybridAnomalyDetector() {}

// Destructor (empty) implementation.
HybridAnomalyDetector::~HybridAnomalyDetector() {}

// createCF function implementation. This function gets all the parameters needed to create a CF.
// if the pearson is higher than SimpleDetector's threshold, than the function will call in implementation
// of it's parent to create a linear CF and push it to cf vector. If not, but higher than 0.5, than it
// will create a circular CF and push it to cf vector. Otherwise, it will do nothing.
void HybridAnomalyDetector::createCF(string f1, string f2, size_t size, float pearson, Point** ps) {
    if (pearson > this->threshold) {
        SimpleAnomalyDetector::createCF(f1, f2, size, pearson, ps);
    } else if (pearson > 0.5) {
        correlatedFeatures circular_cf;
        circular_cf.feature1 = move(f1);
        circular_cf.feature2 = move(f2);
        circular_cf.corrlation = pearson;
        Circle mec = findMinCircle(ps, size);
        circular_cf.center = mec.center;
        circular_cf.radius = mec.radius;
        circular_cf.threshold = 1.1 * mec.radius;
        this->cf.push_back(circular_cf);
    }
}

// isExceptional function implementation. This function gets point p and correlated-features c, and check
// if c's mec's radius is equal to -1, which means that the mec of the given cf (c) is not initialized.
// Given all correlations in this stage (detect stage) are correlated is a sort of way (linear or circular),
// all that remains to be done is to determine whether point p deviates from the norm or not. So, if the
// mec isn't initialized, it means the correlation is linear to the function calls the implementation of
// its parent SimpleAnomalyDetector. Otherwise, it is a circular correlation and the mec must be initialized
// (the radius is not negative) so the function return "true" if point p outside the mec, and false if not.
bool HybridAnomalyDetector::isExceptional(Point p, correlatedFeatures c) {
    if (c.radius == -1) {
        return SimpleAnomalyDetector::isExceptional(p, c);
    } else {
        return (dist(p, c.center) > c.threshold);
    }

}



