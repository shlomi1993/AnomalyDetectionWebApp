// /*
//  * Server.cpp
//  *
//  * Author: Shlomi Ben-Shushan ID: 311408264
//  */

// #include "Server.h"

// // SocketIO's first read function uses unistd.h read() function to return line of string.
// string SocketIO::read() {
//     string str;
//     char buffer = 0;
//     recv(cid, &buffer, sizeof(char), 0);
//     while (buffer != '\n') {
//         str += buffer;
//         recv(cid, &buffer, sizeof(char), 0);
//     }
//     return str;
// }

// // SocketIO's second read function uses unistd.h read() function to get a line of string, and store in in the given *f.
// void SocketIO::read(float* f) {
//     recv(cid, f, sizeof(float),0);
// }

// // SocketIO's first write function gets a string and uses unistd.h write() function to deliver it via network.
// void SocketIO::write(string text) {
//     send(cid, text.c_str(), text.length(), 0);
// }

// // SocketIO's second write function gets a float and uses unistd.h write() function to deliver it via network.
// void SocketIO::write(float f) {
//     stringstream ss;
//     ss << f;
//     string str(ss.str());
//     write(str);
// }

// // Server's constructor implementation.
// Server::Server(int port) throw (const char*) {

//     // Setup socket-stream.
//     if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//         throw system_error(errno, generic_category(), "Couldn't open a new socket\n");

//     // Set address.
//     m_server_addr.sin_family = AF_INET;
//     m_server_addr.sin_addr.s_addr = INADDR_ANY;
//     m_server_addr.sin_port = htons(port);

//     // Bind.
//     if (bind(m_server_fd, (struct sockaddr*)&m_server_addr, sizeof(m_server_addr)) < 0)
//         throw system_error(errno, generic_category(), "Binding failed\n");

//     // Listen.
//     if ((listen(m_server_fd, 5)) < 0)
//         throw system_error(errno, generic_category(), "Listening failed\n");

//     // Stopping flag (for start function).
//     m_stop = false;

// }

// // Server's start function - implemented according to Eli's instructions.
// void Server::start(ClientHandler& ch) throw (const char*){
//     t = new thread([&ch, this]() {
//         socklen_t client_size = sizeof(m_client_addr);
//         while (!m_stop) {
//             alarm(1);
//             int client_id = accept(m_server_fd, (struct sockaddr *) &m_client_addr, &client_size);
//             if (client_id < 0)
//                 throw system_error(errno, generic_category(), "Error accepting new connection");
//             t = new thread([&ch, client_id]() {
//                 ch.handle(client_id);
//                 close(client_id);
//             });
//             alarm(0);
//         }
//         if (close(m_server_fd) < 0)
//             throw system_error(errno, generic_category(), "Server's socket closing failed: ");
//     });
// }

// // Server's stop function - join thread t and set the stopping flag to true.
// void Server::stop() {
//     m_stop = true;
//     t->join(); // do not delete this!
// }

// // Server's destructor.
// Server::~Server() {
//     delete t;
// }


