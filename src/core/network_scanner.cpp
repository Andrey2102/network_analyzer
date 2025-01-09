#include "network_scanner.h"

// Initialize the static instance variable
NetworkScanner* NetworkScanner::_network_scanner = nullptr;

NetworkScanner::NetworkScanner()
{
    // Constructor implementation
}

NetworkScanner::~NetworkScanner()
{
    // Destructor implementation
}

NetworkScanner* NetworkScanner::getInstance()
{
    if (_network_scanner == nullptr) {
        _network_scanner = new NetworkScanner();
    }
    return _network_scanner;
}

// Other method implementations...