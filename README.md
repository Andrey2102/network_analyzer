# Network Management Tool

A Qt-based network analyzer tool for scanning and managing local networks.

## Features

- **Network Scanning**: Identify devices connected to your local network and display their IP and MAC addresses.
- **Network Diagnostics**: Ping devices to check availability and perform traceroute to any specified node.
- **Port Scanning**: Detect open ports on devices and show information about the associated services.
- **Graphical User Interface**: A user-friendly interface with sorting, filtering, and visual network mapping options.
- **Additional Features**:
  - Export results to CSV, JSON, or XML.
  - Customizable scan intervals.
  - Built-in notifications when new devices join the network.

## Requirements

- **C++17** or later
- **Qt 6** (Core, Widgets, and Network modules)
- **CMake 3.16** or later

## Building

1. Clone the repository:
```bash
git clone https://github.com/Andrey2102/network_analyzer.git
cd network_analyzer
```

2. Create build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
make
```