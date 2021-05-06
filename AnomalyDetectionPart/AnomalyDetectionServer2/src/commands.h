// /*
//  * commands.h
//  *
//  * Author: Shlomi Ben-Shushan ID: 311408264
//  */

// #ifndef COMMANDS_H_
// #define COMMANDS_H_

// #include <cstring>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <sys/socket.h>
// #include "HybridAnomalyDetector.h"

// using namespace std;

// // This struct represent a segment on the time-line on which an anomaly occur.
// struct Segment {
//     size_t start;
//     size_t end;
//     Segment(long s, long e): start(s), end(e) {};
// };

// // This struct contains variables and object that needs to be shared by different commands.
// struct SharedState {
//     float threshold;
//     vector<AnomalyReport> reports;
//     size_t samples;
//     bool stop;
//     SharedState() {
//         threshold = 0.9;
//         samples = 0;
//         stop = false;
//     }
// };

// // This class - the root IO class.
// class DefaultIO {
// public:
//     virtual string read() = 0;
//     virtual void write(string text) = 0;
//     virtual void write(float f) = 0;
//     virtual void read(float* f) = 0;
//     virtual ~DefaultIO()= default;
// };

// // StandardIO - input and output via keyboard and terminal.
// class StandardIO : public DefaultIO {
// public:
//     string read() override {
//         string str;
//         cin >> str;
//         return str;
//     }
//     void write(string text) override { cout << text; }
//     void write(float f) override { cout << f; }
//     void read(float* f) override { cin >> *f; }
// };

// // Command class - implementation of Command pattern. I added description string.
// class Command {
// protected:
//     DefaultIO* dio;
// public:
//     string description;
//     explicit Command(DefaultIO* dio) : dio(dio) {}
//     virtual void execute(SharedState* sharedState) = 0;
//     virtual ~Command() = default;
// };

// // Upload command - gives the user the ability to send data to the server.
// class Upload : public Command {
// public:
//     explicit Upload(DefaultIO *dio) : Command(dio) {
//         this->description = "upload a time series csv file\n";
//     }
//     void execute(SharedState* sharedState) override {

//         // Set strings.
//         string line;
//         string file_name[2] = {"anomalyTrain.csv", "anomalyTest.csv"};
//         string type[2] = {"train", "test"};

//         // Ask the user for train-file, and than test-file.
//         for (int i = 0; i < 2; ++i) {
//             dio->write("Please upload your local " + type[i] + " CSV file.\n");
//             ofstream out(file_name[i]);
//             while ((line = dio->read()) != "done")
//                 out << line << "\n"; // Create anomalyTrain.csv and anomalyTest.csv files at the server.
//             out.close();
//             dio->write("Upload complete.\n");
//         }

//     }

// };

// // Settings command - allows the user to change correlation threshold.
// class Settings : public Command {
// public:
//     explicit Settings(DefaultIO *dio) : Command(dio) {
//         this->description = "algorithm settings\n";
//     }
//     void execute(SharedState* sharedState) override {

//         // Tell the user the current threshold and ask for a new one.
//         dio->write("The current correlation threshold is ");
//         dio->write(sharedState->threshold);
//         dio->write("\n");
//         dio->write("Type a new threshold\n");

//         // If user's input is valid, than change the threshold (by pointer) to it. Otherwise, ask for correct input.
//         try {
//             float input = stof(dio->read());
//             if (0 <= input && input <= 1) {
//                 sharedState->threshold = input;
//             } else {
//                 dio->write("please choose a value between 0 and 1.\n");
//                 execute(sharedState);
//             }
//         } catch (const exception& e) {
//             dio->write("please choose a value between 0 and 1.\n");
//             execute(sharedState);
//         }

//     }

// };

// // Detect command - runs the Anomaly Detection algorithm.
// class Detect: public Command {
// public:
//     explicit Detect(DefaultIO *dio) : Command(dio) {
//         this->description = "detect anomalies\n";
//     }
//     void execute(SharedState* sharedState) override {

//         // Create Time-Series objects for train and test csv giles.
//         TimeSeries train("anomalyTrain.csv");
//         TimeSeries test("anomalyTest.csv");

//         // Set number of samples for use of other commands.
//         sharedState->samples = test.getDataSize();

//         // Create an Hybrid Anomaly Detector with updated threshold.
//         HybridAnomalyDetector had;
//         had.setCorrelationThreshold(sharedState->threshold);

//         // Learn normal correlations from anomalyTrain file.
//         had.learnNormal(train);

//         // Detect anomalies in anomalyTest file.
//         sharedState->reports = had.detect(test);

//         // Inform user the detection was completed.
//         dio->write("anomaly detection complete.\n");

//     }

// };

// // Results command - send\display the anomaly detection results.
// class Results: public Command {
// public:
//     explicit Results(DefaultIO *dio) : Command(dio) {
//         this->description = "display results\n";
//     }
//     void execute(SharedState* sharedState) override {
//         // Using shared-state to print each timeStep and description.
//         for (auto & ar : sharedState->reports) {
//             string res = to_string(ar.timeStep) + "\t" + ar.description;
//             dio->write(res + "\n");
//         }
//         dio->write("Done.\n");
//     }
// };

// // Analyze command - allows the user to send local anomalies file to check true\false positive rate.
// class Analyze: public Command {
// public:
//     Analyze(DefaultIO *dio) : Command(dio) {
//         this->description = "upload anomalies and analyze results\n";
//     }
//     // This comparator returns true if segment a starts before b, or false otherwise.
//     static bool comparator(const Segment& a, const Segment& b) {
//         return (a.start < b.start);
//     }
//     // This function gets a pointer to AR vector and returns a vector of AR unite segments.
//     static vector<Segment> getAnomalySegments(vector<AnomalyReport> *ar) {

//         // Set variables.
//         size_t size = ar->size();
//         vector<Segment> sv;
//         size_t start, end;
//         size_t i, j;

//         // This nested loop checks for Anomaly Reports with consecutive time-steps.
//         // Than it creates a Segment of anomaly and push it the the return vector.
//         for (i = 0; i < size; ++i) {
//             start = ar->at(i).timeStep;
//             end = ar->at(i).timeStep;
//             for (j = i + 1; j < size; ++j) {
//                 size_t ts = ar->at(j).timeStep;
//                 if (ts == end + 1) {
//                     ++end;
//                 } else {
//                     break;
//                 }
//             }
//             sv.emplace_back(start, end);
//             i = j - 1;
//         }
//         // return a vector of anomalous segments.
//         return sv;

//     }

//     void execute(SharedState* sharedState) override {

//         // Set variables as described in the instructions, and other variables.
//         size_t P = 0, TP = 0, FP = 0;
//         size_t N = sharedState->samples;
//         string line;
//         size_t start, end;

//         // Get as - a vector of anomalous segments - than sort it and get its size.
//         vector<Segment> as = getAnomalySegments(&sharedState->reports);
//         sort(as.begin(), as.end(), comparator);
//         size_t num_of_segments = as.size();

//         // Ask the user for a local anomalies file.
//         dio->write("Please upload your local anomalies file.\n");

//         // Create a vector for the "real" anomalies.
//         vector<Segment> anomalies;

//         // This loop will add a segment (to anomalies) for each anomaly the found in the local file.
//         // In addition, it counts the Positive anomalies and update P variable.
//         while ((line = dio->read()) != "done") {
//             ++P;
//             start = stof(line.substr(0, line.find(',')));
//             end = stof(line.substr (line.find(',') + 1));
//             anomalies.emplace_back(start, end);
//         }

//         // Now that we have anomalies vector without duplications, we can sort it and get its size.
//         sort(anomalies.begin(), anomalies.end(), comparator);
//         size_t num_of_anomalies = anomalies.size();

//         // The following nested loop advance FP and TP variables according to the instructions.
//         size_t j = 0;
//         for (size_t i = 0; i < num_of_anomalies; ++i) {
//             start = anomalies.at(i).start;
//             end = anomalies.at(i).end;
//             while (j < num_of_segments && as.at(j).start <= end) {
//                 if (as.at(j).end < start)
//                     ++FP;
//                 else
//                     ++TP;
//                 ++j;
//             }
//             N -= (end - start + 1);
//         }
//         FP += (num_of_segments - j);

//         // Prepare results for output..
//         float true_positive_rate = floor(1000 * ((float)TP / P)) / 1000;
//         float false_alarm_rate = floor(1000 * ((float)FP / N)) / 1000;

//         // Write results.
//         dio->write("Upload complete.\n");
//         dio->write("True Positive Rate: ");
//         dio->write(true_positive_rate);
//         dio->write("\n");
//         dio->write("False Positive Rate: ");
//         dio->write(false_alarm_rate);
//         dio->write("\n");

//     }
// };

// // Exit command - end the program.
// class Exit: public Command {
// public:
//     Exit(DefaultIO *dio) : Command(dio) {
//         this->description = "exit\n";
//     }
//     void execute(SharedState* sharedState) override {
//         sharedState->stop = true;
//     }
// };

// #endif /* COMMANDS_H_ */
