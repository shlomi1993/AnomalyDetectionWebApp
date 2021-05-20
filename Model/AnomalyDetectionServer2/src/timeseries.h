/*
 * TimeSeries.h
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <iostream>
#include <string.h>
#include <fstream>
#include<map>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

// TimeSeries class.
class TimeSeries{

private:

    // A ts should "know" its features names, their data and the number of samples in the ts (data size).
    vector<string> m_features;
	map<string,vector<float>> m_data;
	size_t m_data_size;

public:

    // Constructor.
	TimeSeries(const char* CSVfileName);

    // getFeatureData function gets a feature name and returns the corresponded vector of data.
	const vector<float>& getFeatureData(const string& feature) const;

    // getFeaturesNames functions returns the vector that holds the names of the features.
    const vector<string>& getFeaturesNames() const;

    // getDataSize returns the size, the number of samples, in the ts (lines in CSV, without headers).
    size_t getDataSize() const;

    // Destructor.
	~TimeSeries() = default;


};



#endif /* TIMESERIES_H_ */
