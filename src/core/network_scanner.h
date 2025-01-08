#ifndef _NETWORK_SCANNER_H_
#define _NETWORK_SCANNER_H_

#include <QVector>
#include "network_device.h"

class NetworkScanner
{
private:
    QVector <NetworkDevice> _devices;
    NetworkDevice _curent_device;

    // Private constructor to prevent instantiation
    NetworkScanner(/* args */);
    ~NetworkScanner();

    // Delete copy constructor and assignment operator
    NetworkScanner(const NetworkScanner&) = delete;
    NetworkScanner& operator=(const NetworkScanner&) = delete;

    // Static instance of the class
    static NetworkScanner* _network_scanner;

public:
    // Static method to get the instance of the class
    static NetworkScanner* getInstance();

};

#endif // _NETWORK_SCANNER_H_