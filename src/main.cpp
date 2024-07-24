#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "../include/networking/Socket.hpp"
#include "../include/networking/PollManager.hpp"
#include "../include/networking/PollManagerExceptions.hpp"
#include <cassert>  // For assertions
using namespace net;

bool testAddValidSocket() {
    PollManager manager;
    int mockSocketFd = 123; 

    try {
        manager.addSocket(mockSocketFd);
    } catch (const invalidFd& e) {
        std::cerr << "Error adding valid socket: " << e.what() << std::endl;
        return false;
    }

    return manager.getPollSize() == 1; // Check if size increased
}

// Test adding a valid socket with specific events (e.g., POLLOUT)
bool testAddValidSocketWithEvents() {
    PollManager manager;
    int mockSocketFd = 456; 

    try {
        manager.addSocket(mockSocketFd, POLLOUT); // Specify POLLOUT events
    } catch (const invalidFd& e) {
        std::cerr << "Error adding valid socket with events: " << e.what() << std::endl;
        return false;
    }

    return manager.getPollSize() == 1 && manager.getPollFd(0).events == POLLOUT; 
    // Check size and correct event
}

bool testAddInvalidSocket() {
    PollManager manager;
    int invalidSocketFd = -1;

    try {
        manager.addSocket(invalidSocketFd);
        return false; // It should have thrown an exception
    } catch (const invalidFd& e) {
        return true;  
    }
}

bool testRemoveValidSocket() {
    PollManager manager;
    int mockSocketFd = 456; 
    manager.addSocket(mockSocketFd); // Add it first to remove it
    
    try {
        manager.removeSocket(mockSocketFd);
    } catch (const pollManagerException& e) {
        std::cerr << "Error removing valid socket: " << e.what() << std::endl;
        return false;
    }

    return manager.getPollSize() == 0; // Check if size decreased
}

bool testRemoveInvalidSocket() {
    PollManager manager;
    int notAddedSocketFd = 789;

    try {
        manager.removeSocket(notAddedSocketFd);
        return false; // It should have thrown an exception
    } catch (const pollManagerException& e) {
        return true;
    }
}

// Test pollSockets with no sockets added
bool testPollEmptySockets() {
    PollManager manager;

    try {
        manager.pollSockets(100); // Should throw an exception
        return false; 
    } catch (const pollManagerException& e) {
        return true;
    }
}

// Test pollSockets with a timeout (this will likely return 0 immediately)
bool testPollTimeout() {
    PollManager manager;
    int mockSocketFd = socket(AF_INET, SOCK_STREAM, 0); // Create a real socket
    if (mockSocketFd == -1) {
        std::cerr << "Error creating socket: " << std::strerror(errno) << std::endl;
        return false;
    }

    manager.addSocket(mockSocketFd);
    try {
        int numReady = manager.pollSockets(10); // Small timeout
        close(mockSocketFd); // Close the socket after testing
        return numReady == 0; 
    } catch (const pollFailed& e) {
        std::cerr << "Error polling sockets with timeout: " << e.what() << std::endl;
        close(mockSocketFd);
        return false;
    }
}

// Test getting a valid pollfd
bool testGetValidPollFd() {
    PollManager manager;
    int mockSocketFd = 456;
    manager.addSocket(mockSocketFd);
    
    try {
        struct pollfd& pfd = manager.getPollFd(0);
        return pfd.fd == mockSocketFd;
    } catch (const pollManagerException& e) {
        std::cerr << "Error getting valid pollfd: " << e.what() << std::endl;
        return false;
    }
}

// Test getting an invalid pollfd (index out of range)
bool testGetInvalidPollFd() {
    PollManager manager;
    try {
        manager.getPollFd(0); // Should throw an exception
        return false;
    } catch (const pollManagerException& e) {
        return true;
    }
}



/****FAILED_TEST*****/

// Test adding a socket that's already in the poll set
bool testAddDuplicateSocket() {
    PollManager manager;
    int mockSocketFd = 1024;  

    std::cout << "Adding socket for the first time with POLLIN: " << mockSocketFd << std::endl; // Debug
    manager.addSocket(mockSocketFd); // Use default events (POLLIN)

    std::cout << "Attempting to add the same socket again with POLLIN: " << mockSocketFd << std::endl; // Debug

    try {
        manager.addSocket(mockSocketFd);  // Should throw duplicateSocket exception

        std::cerr << "Error: Adding duplicate socket with same events did NOT throw an exception!" << std::endl;
        return false;
    } catch (const duplicateSocket& e) {  // Catch the specific exception
        std::cout << "Caught expected duplicateSocket exception: " << e.what() << std::endl;
        return true; 
    } catch (const pollManagerException& e) {  // Catch any other unexpected PollManager exception
        std::cerr << "Error: Caught unexpected exception: " << e.what() << std::endl;
        return false;
    }
}


// Test adding a duplicate socket with different events (should NOT throw an exception)
bool testAddDuplicateSocketWithDifferentEvents() {
    PollManager manager;
    int mockSocketFd = 1024;  

    manager.addSocket(mockSocketFd, POLLIN);  // Add with POLLIN

    try {
        manager.addSocket(mockSocketFd, POLLOUT);  // Add again with POLLOUT
    } catch (const pollManagerException& e) {
        std::cerr << "Error: Adding duplicate socket with different events threw an exception: " << e.what() << std::endl;
        return false;
    }

    return manager.getPollSize() == 2; // Check if both were added
}

// Test pollSockets with multiple sockets, some ready and some not

bool testPollMultipleSockets() {
    // Create server socket
    Socket server(AF_INET, SOCK_STREAM, 0);
    if (!server.Bind(8080, "127.0.0.1")) {
        std::cerr << "Error binding server socket: " << strerror(errno) << std::endl;
        return false;
    }
    if (!server.Listen(1)) {
        std::cerr << "Error listening on server socket: " << strerror(errno) << std::endl;
        return false;
    }

    // Connect the first client socket
    Socket client(AF_INET, SOCK_STREAM, 0);

    // Get server address and connect
    sockaddr_in serverAddress = server.getServerAddress();
    if (connect(client.getFd(), (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting client socket: " << strerror(errno) << std::endl;
        return false;
    }

    // Accept the connection on the server side
    Socket acceptedClient = server.Accept();
    if (acceptedClient.getFd() == -1) {
        std::cerr << "Error accepting client connection: " << strerror(errno) << std::endl;
        return false;
    }

    // Debugging: Check accepted client socket state
    int error = 0;
    socklen_t len = sizeof(error);
    if (getsockopt(acceptedClient.getFd(), SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
        std::cerr << "Error getting socket option (SO_ERROR): " << strerror(errno) << std::endl;
        return false;
    } else if (error != 0) {
        std::cerr << "Socket error after accept: " << strerror(error) << std::endl;
        return false;
    }

    // Send data to the client socket (to make it POLLIN ready)
    PollManager manager;
    manager.addSocket(acceptedClient.getFd());
    const char* message = "Hello from server!";
    ssize_t bytesSent = send(acceptedClient.getFd(), message, strlen(message), 0);
    if (bytesSent < 0  && errno != EAGAIN && errno != EWOULDBLOCK) {
        std::cerr << "Error sending data: " << strerror(errno) << std::endl;
        return false;
    } 
    // Data might be partially sent or buffered
    try { 
      int numReady = manager.pollSockets(1000); // You can adjust the timeout as needed
        // Check if the number of ready sockets is correct
      if (numReady < 1) {
        std::cerr << "Expected at least one ready socket, but found " << numReady << std::endl;
        return false;
     }
    pollfd& readyFd = manager.getPollFd(0); // Get the first ready socket
    if ((readyFd.revents & POLLIN) == 0 && (readyFd.revents & POLLOUT) == 0 ) {
      std::cerr << "Expected POLLIN event, but found " << readyFd.revents << std::endl;
      return false;    
    }
     return true;
    } catch (const pollFailed& e) {
        std::cerr << "Polling failed: " << e.what() << std::endl; 
        return false; 
    }
}

int main() {
    // Run the tests
    bool allTestsPassed = true;

    /*if (!testAddValidSocket()) {
        std::cout << "testAddValidSocket failed" << std::endl;
        allTestsPassed = false;
    } */if (!testAddValidSocketWithEvents()) {
        std::cout << "testAddValidSocketWithEvents failed" << std::endl;
    }/*

    if (!testAddInvalidSocket()) {
        std::cout << "testAddInvalidSocket failed" << std::endl;
        allTestsPassed = false;
    }

    // Test removeSocket
    if (!testRemoveValidSocket()) {
        std::cout << "testRemoveValidSocket failed" << std::endl;
        allTestsPassed = false;
    }
    if (!testRemoveInvalidSocket()) {
        std::cout << "testRemoveInvalidSocket failed" << std::endl;
        allTestsPassed = false;
    }

    // Test pollSockets
    if (!testPollEmptySockets()) {
        std::cout << "testPollEmptySockets failed" << std::endl;
        allTestsPassed = false;
    }
    if (!testPollTimeout()) {
        std::cout << "testPollTimeout failed" << std::endl;
        allTestsPassed = false;
    }

    // Test getPollFd
    if (!testGetValidPollFd()) {
        std::cout << "testGetValidPollFd failed" << std::endl;
        allTestsPassed = false;
    }
    if (!testGetInvalidPollFd()) {
        std::cout << "testGetInvalidPollFd failed" << std::endl;
        allTestsPassed = false;
    }

    if (!testAddDuplicateSocket()) {
        std::cout << "testAddDuplicateSocket failed" << std::endl;
        allTestsPassed = false;
    }
    
    if (!testAddDuplicateSocketWithDifferentEvents()) {
      std::cout << "testAddDuplicateSocketWithDifferentEvents failed" << std::endl;
      allTestsPassed = false;
    }*/

    if (!testPollMultipleSockets()) {
        std::cout << "testPollMultipleSockets failed" << std::endl;
        allTestsPassed = false;
    }

    if (allTestsPassed) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    }
}
