// /*
//  * CLI.cpp
//  *
//  * Author: Shlomi Ben-Shushan ID: 311408264
//  */

// #include "CLI.h"

// // CLI constructor.
// CLI::CLI(DefaultIO* dio) : dio(dio) {
//     this->dio=dio;
//     commands.push_back(new Upload(dio));
//     commands.push_back(new Settings(dio));
//     commands.push_back(new Detect(dio));
//     commands.push_back(new Results(dio));
//     commands.push_back(new Analyze(dio));
//     commands.push_back(new Exit(dio));
// }

// // Start function ignite the program.
// void CLI::start() {

//     // Creates a shared-state object.
//     SharedState sharedState;

//     // This loop stops when Exit command is called and set stop flag to true.
//     while (!sharedState.stop) {

//         // First output.
//         dio->write("Welcome to the Anomaly Detection Server.\n");
//         dio->write("Please choose an option:\n");

//         // Print menu.
//         size_t size = commands.size();
//         for (size_t i = 0; i < size; ++i) {
//             string s(to_string(i + 1) + "." + commands.at(i)->description);
//             dio->write(s);
//         }

//         // Executes the command requested by the user.
//         try {
//             int i = stoi(dio->read());
//             commands[i - 1]->execute(&sharedState);
//         } catch (const exception& e) {
//             dio->write("Please choose an option from 1 to 6.\n");
//         }

//     }

// }

// // Destructor.
// CLI::~CLI() {
//     for(auto & command : commands)
//         delete command;
// }


