/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */

#include "SimpleAnomalyDetector.h"
#include <iostream>

using namespace std;

// Constructor - initialize the threshold of a linear correlation.
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->threshold = 0.9;
}

// Destructor is empty because allocated memory deleted during process (points).
SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

// createCF function decide if gets all the parameters needed to create a CF. It does so only if the
// pearson is higher than the threshold. Once created, the CF is pushed to cf vector.
void SimpleAnomalyDetector::createCF(string f1, string f2, size_t size, float pearson, Point** ps) {

    if (pearson > this->threshold) {

        correlatedFeatures linear_cf;
        linear_cf.feature1 = move(f1);
        linear_cf.feature2 = move(f2);
        linear_cf.corrlation = pearson;
        linear_cf.lin_reg = linear_reg(ps, size);

        // find max deviation (using dev function) to set the threshold.
        float max_deviation = 0;
        for (size_t i = 0; i < size; i++)
            max_deviation = max(max_deviation, dev(*ps[i], linear_cf.lin_reg));
        linear_cf.threshold = 1.1 * max_deviation; // take 10% safety-margin.

        this->cf.push_back(linear_cf);

    }

}

// isExceptional function determine if point p is exceptional to the correlation by comparing
// the deviation from the cf linear reg line, to the correlation threshold.
bool SimpleAnomalyDetector::isExceptional(Point p, correlatedFeatures c) {
    return (dev(p, c.lin_reg) > c.threshold);
}


// learnNormal function - gets a time-series and save high correlations between features in a vector.
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {

    // Initialize the variables required for the process.
    vector<string> features = ts.getFeaturesNames(); // the names of the features in the ts.
    size_t data_size = ts.getDataSize(); // the number of samples in each feature,
    size_t num_of_features = features.size(); // the number of features in the ts.
    float data[num_of_features][data_size]; // a 2d array to store all the data and access it easily.

    // This loop fill data array with... data.
    for (size_t i = 0; i < num_of_features; ++i) {
        for (size_t j = 0; j < data_size; ++j) {
            vector<float> data_from = ts.getFeatureData(features[i]);
            data[i][j] = data_from[j];
        }
    }

    // This loop check for correlations and call createCF function to create a correlation-features,
    // but only if the correlation is above the relevant threshold.
    for (size_t i = 0; i < num_of_features; ++i) {

        float max_pearson = 0;
        size_t most_correlated_index = 0;

        // Check for correlation with any other feature (avoid duplications).
        for (size_t j = i + 1; j < num_of_features; ++j){
            float new_pearson = abs(pearson(data[i],data[j],data_size));
            if (new_pearson > max_pearson){
                max_pearson = new_pearson;
                most_correlated_index = j;
            }
        }
        string f1 = features[i];
        string f2 = features[most_correlated_index];

        // Retrieve data of the two correlated features.
        vector<float> f1_data = ts.getFeatureData(f1);
        vector<float> f2_data = ts.getFeatureData(f2);

        // Create an array of points by the values of the two correlated features.
        auto** ps = new Point *[data_size];
        for (size_t k = 0; k < data_size; ++k)
            ps[k] = new Point(f1_data[k], f2_data[k]);

        // Call createCF function to create the relevant cf (linear or circular).
        // the creation will take place only if the correlation is above the relevant threshold.
        createCF(f1, f2, data_size, max_pearson, ps);

        // Free memory allocated for points.
        for (size_t k = 0; k < data_size; ++k)
            delete ps[k];
        delete[] ps;

    }

}

// detect function - gets a NEW time-series report any anomaly found.
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {

    // Create return vector and a local copy of the new TS.
    vector<AnomalyReport> anomalies;

    // Gets sizes.
    size_t num_of_samples = ts.getDataSize();
    size_t num_of_correlations = this->cf.size();

    // This loop look for a deviation from liner_reg line of each CF.
    for (size_t i = 0; i < num_of_correlations; ++i) {

        // Set features names.
        string f1 = this->cf[i].feature1;
        string f2 = this->cf[i].feature2;

        // Get data of the correlated features from the new TS.
        vector<float> data1 = ts.getFeatureData(f1);
        vector<float> data2 = ts.getFeatureData(f2);

        // Look for a deviation that crossed the threshold and add it to the return vector.
        for (size_t j = 0; j < num_of_samples; ++j)
            if (isExceptional(Point(data1[j], data2[j]), this->cf[i]))
                anomalies.emplace_back(f1 + " - " + f2, j + 1);

    }

    return anomalies;

}




