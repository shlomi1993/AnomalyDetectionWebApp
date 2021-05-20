// /*
//  * Server.h
//  *
//  * Author: Shlomi Ben-Shushan ID: 311408264
//  */

// #ifndef SERVER_H_
// #define SERVER_H_

// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <pthread.h>
// #include <thread>
// #include <sys/socket.h>
// #include <iostream>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <system_error>
// #include "CLI.h"
// #include "commands.h"

// using namespace std;

// // SocketIO - input and output via network. Implementation on Server.cpp.
// class SocketIO : public DefaultIO {
// private:
//     int cid; // A SocketIO should have the servers file-descriptor.
// public:
//     SocketIO(int client_id) : cid(client_id) {}
//     virtual string read();
//     virtual void read(float* f);
//     virtual void write(string text);
//     virtual void write(float f);
// };

// // ClientHandler abstract class.
// class ClientHandler {
// public:
//     virtual void handle(int clientID) = 0;
// };

// // AnomalyDetectionHandler class implementation.
// class AnomalyDetectionHandler : public ClientHandler {
// public:
//     virtual void handle(int client_id) {
//         SocketIO sio(client_id);     // Create SocketIO.
//         CLI cli(&sio);              // Initialize CLI with the SocketIO.
//         cli.start();                // Start Anomaly Detection CLI.
//     }
// };

// // Server class declaration. Implemented on Server.cpp.
// class Server {
//     thread* t{};    // the thread to run the start() method in
//     int m_server_fd;
//     sockaddr_in m_server_addr{};
//     sockaddr_in m_client_addr{};
//     bool m_stop;
// public:
//     Server(int port) throw (const char*);
//     virtual ~Server();
//     void start(ClientHandler& ch) throw (const char*);
//     void stop();
// };

// #endif /* SERVER_H_ */
