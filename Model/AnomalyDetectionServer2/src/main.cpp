/*
 * main.cpp
 *
 * Author: Shlomi Ben-Shushan ID: 311408264
 */
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "HybridAnomalyDetector.h"

using namespace std;

void linearAnomalyDetection(float t) {

	string output = "";
	ofstream file;
	file.open("anomalies.txt");

	ifstream train, test;
	train.open("anomalyTrain.csv");
	test.open("anomalyTest.csv");

	if (!train && !test) {
		file << "Error: couldn't find anomalyTrain.csv file nor anomalyTest.csv file.";
		return;
	}
	else if (!train) {
		file << "Error: couldn't find anomalyTrain.csv file.";
		test.close();
		return;
	}
	else if (!test) {
		file << "Error: couldn't find anomalyTest.csv file.";
		train.close();
		return;
	}
	else {
		train.close();
		test.close();
	}

	SimpleAnomalyDetector ad;
	ad.setCorrelationThreshold(t);

	TimeSeries ts("anomalyTrain.csv");
	ad.learnNormal(ts);

	TimeSeries ts2("anomalyTest.csv");
	vector<AnomalyReport> v = ad.detect(ts2);

	int size = v.size();
	if (size == 0) {
		output = "No linear anomalies found.";
	}
	else {
		for (int i = 0; i < size; i++) {
			output += to_string(v[i].timeStep);
			output += "\t";
			output += v[i].description;
			output += "\n";
		}
	}

	file << output;
	file.close();
}

void hybridAnomalyDetection(float t) {

	string output = "";
	ofstream file;
	file.open("anomalies.txt");

	ifstream train, test;
	train.open("anomalyTrain.csv");
	test.open("anomalyTest.csv");

	if (!train && !test) {
		file << "Error: couldn't find anomalyTrain.csv file nor anomalyTest.csv file.";
		return;
	}
	else if (!train) {
		file << "Error: couldn't find anomalyTrain.csv file.";
		test.close();
		return;
	}
	else if (!test) {
		file << "Error: couldn't find anomalyTest.csv file.";
		train.close();
		return;
	}
	else {
		train.close();
		test.close();
	}

	HybridAnomalyDetector ad;
	ad.setCorrelationThreshold(t);

	TimeSeries ts("anomalyTrain.csv");
	ad.learnNormal(ts);

	TimeSeries ts2("anomalyTest.csv");
	vector<AnomalyReport> v = ad.detect(ts2);

	int size = v.size();
	if (size == 0) {
		output = "No circular anomalies found.";
	}
	else {
		for (int i = 0; i < size; i++) {
			output += to_string(v[i].timeStep);
			output += "\t";
			output += v[i].description;
			output += "\n";
		}
	}

	file << output;
	file.close();
}


int main(int argc, char *argv[]){
	// int port;
	// try {
	//     port = stoi(argv[1]);
	// } catch (const exception &e) {
	//     port = 6500;
	// }
	// try {
	// 	AnomalyDetectionHandler adh;
	// 	Server server(port);
	// 	server.start(adh);
	// 	server.stop();
	// } catch (const char* s) {}
	return 0;
}