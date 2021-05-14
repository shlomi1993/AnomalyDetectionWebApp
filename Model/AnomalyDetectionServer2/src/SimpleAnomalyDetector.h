/*
 * SimpleAnomalyDetector.h
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <utility>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

// Correlated features struct - include names of correlated features and correlation details.
struct correlatedFeatures {
    string feature1,feature2;
    float corrlation;
    Line lin_reg;
    Point center;
    float radius = -1; // radius -1 means linear correlations.
    float threshold;
};

// SimpleAnomalyDetector class.
class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {

protected:

    // A simple-detector have a vector of correlated-features and threshold.
    // This members have to be shared with inheritance classes so it is under protected.
    vector<correlatedFeatures> cf;
    float threshold;

public:

    // Constructor.
    SimpleAnomalyDetector();

    // Destructor.
    virtual ~SimpleAnomalyDetector();

    // learnNormal function - gets a time-series and save high correlations between features in a vector.
    virtual void learnNormal(const TimeSeries& ts);

    // detect function - gets a NEW time-series report any anomaly found.
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    // getNormalModel function - return cf member.
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }

    // createCF - creates the linear or circular cf if the correlation is higher than the relevant threshold.
    virtual void createCF(string f1, string f2, size_t size, float pearson, Point** ps);

    // isExceptional - tell if point p is exceptional to the correlated features c (detect helper).
    virtual bool isExceptional(Point p, correlatedFeatures c);

    // setCorrelationThreshold - allows to change the correlation threshold of the Anomaly Detector.
    void setCorrelationThreshold(float t) { this->threshold = t; }


};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
