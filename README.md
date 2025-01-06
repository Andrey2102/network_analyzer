
Network Management Tool
Project Description
This tool is a network analyzer that allows users to scan their local network and display information about all connected devices. The program can be useful for network administrators, developers, and home users who want to know which devices are connected to their network.

Features
Network Scanning:

Identify all devices connected to the local network.
Display IP addresses, MAC addresses, and device manufacturers (via MAC lookup).
Network Diagnostics:

Ping IP addresses to check their availability.
Perform traceroute to a specified node.
Port Scanning:

Detect open ports on selected devices.
Show information about services associated with these ports.
Graphical User Interface:

A list of devices with sorting and filtering options.
A visual network map showing devices as nodes.
Additional Features:

Export results to files (CSV, JSON, XML).
Customizable scan intervals.
Built-in notifications (e.g., when a new device joins the network).
Technologies
Qt Widgets or Qt Quick: For developing a user-friendly interface.
libpcap or similar libraries (e.g., WinPcap): For capturing packets and analyzing network traffic.
Additional utilities/services: Use of system utilities (ping, traceroute) for diagnostics.
SQLite or files: For storing scan history.
Interface Structure
Main Window:
Table listing devices: IP, MAC, manufacturer, availability status.
Control buttons: "Scan," "Ping," "Traceroute," "Port Scan."
Details Window:
Detailed information about a selected device: active ports, ping response time.
Settings: IP address range for scanning, scan type (quick/deep).
Example User Scenario
The user launches the application and clicks the "Scan" button.
The program displays a list of all devices in the local network with their IP and MAC addresses.
The user selects a device and clicks "Ping" to check its availability.
If necessary, the user starts a port scan to analyze the device's activity.
Results are saved to a file for further analysis.
Potential Future Enhancements
Mobile Version: Create a cross-platform application for mobile devices.
Advanced Graphs: Visualize network load and data transmission speed.
SNMP Support: Retrieve additional information about network devices.
Real-World Benefits
Helps manage local networks (e.g., office or home environments).
Improves security by detecting unauthorized connections.
Facilitates troubleshooting and resolving network issues.
This project can be both educational and commercially viable.