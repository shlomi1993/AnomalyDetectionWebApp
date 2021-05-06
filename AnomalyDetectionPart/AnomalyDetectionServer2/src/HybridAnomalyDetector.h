/*
 * HybridAnomalyDetector.h
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

// HybridAnomalyDetector class.
class HybridAnomalyDetector: public SimpleAnomalyDetector {

public:

    // Constructors.
	HybridAnomalyDetector();

    // Destructor.
    virtual ~HybridAnomalyDetector();

	// Override createCF function create a circular CF, if needed.
    virtual void createCF(string f1, string f2, size_t size, float pearson, Point** ps);

    // Override isExceptional function to check if p is outside to c's mec.
    virtual bool isExceptional(Point p, correlatedFeatures c);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
