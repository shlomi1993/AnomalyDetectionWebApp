/*
 * TimeSeries.cpp
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */

#include "timeseries.h"

// Constructor.
TimeSeries::TimeSeries(const char *CSVfileName) {

    // Open input stream.
    ifstream in(CSVfileName);

    // Add m_features and empty float-vectors to the map.
    string first_line, feature;
    in >> first_line;
    stringstream feature_ss(first_line);
    while (getline(feature_ss, feature, ',')) {
        vector<float> feature_data;
        this->m_data[feature] = feature_data;
        this->m_features.push_back(feature);
    }

    // Fill float-vectors with data.
    while (!in.eof()) {
        string row, cell;
        in >> row;
        stringstream row_ss(row);
        size_t i = 0;
        while (getline(row_ss, cell, ',')) {
            this->m_data[m_features[i++]].push_back(stof(cell));
        }
    }

    // Close input stream.
    in.close();

    // Set data-size member.
    this->m_data_size = this->m_data[m_features[0]].size();

}

// getFeatureData function gets a feature name and returns the corresponded vector of data.
const vector<float> &TimeSeries::getFeatureData(const string& feature) const {
    return this->m_data.at(feature);
}

// getFeaturesNames functions returns the vector that holds the names of the features.
const vector<string> &TimeSeries::getFeaturesNames() const {
    return this->m_features;;
}

// getDataSize returns the size, the number of samples, in the ts (lines in CSV, without headers).
size_t TimeSeries::getDataSize() const {
    return this->m_data_size;
}