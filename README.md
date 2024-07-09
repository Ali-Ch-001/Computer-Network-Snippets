# Computer Network Snippets in C

## Description
This repository contains a collection of computer network code snippets written in C. These examples demonstrate various networking concepts and protocols, aimed at helping learners understand and implement network programming in C.

## Features
- Basic socket programming examples
- Implementations of common networking protocols
- Network configuration scripts
- Troubleshooting and diagnostic tools

## Requirements
- GCC compiler or any compatible C compiler
- Network access for testing network-related scripts
- Administrative privileges for executing certain network configuration scripts

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/Ali-Ch-001/Computer-Network-Snippets.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Computer-Network-Snippets
   ```

## Usage
1. Choose the desired network snippet from the repository.
2. Compile the C code using your preferred C compiler. For example, with GCC:
   ```bash
   gcc snippet.c -o snippet
   ```
3. Run the compiled executable:
   ```bash
   ./snippet
   ```

### Example
To run a basic TCP client-server program:
1. Navigate to the `tcp-client-server` directory:
   ```bash
   cd tcp-client-server
   ```
2. Compile and start the server:
   ```bash
   gcc server.c -o server
   ./server
   ```
3. In a new terminal window, compile and start the client:
   ```bash
   gcc client.c -o client
   ./client
   ```

## Snippets
The repository includes the following snippets:

### Socket Programming
- **TCP Client-Server**: Basic TCP client-server communication.
- **UDP Communication**: Basic UDP communication example.

### Networking Protocols
- **HTTP Request**: Sending an HTTP GET request.
- **DNS Query**: Performing a DNS query using sockets.
- **FTP Client**: Basic FTP client implementation.

### Network Configuration
- **IP Configuration**: Script to configure IP addresses on Linux.
- **Firewall Setup**: Example iptables configuration for basic firewall rules.

### Troubleshooting
- **Ping Test**: Script to perform a ping test to a specified host.
- **Traceroute**: Implementing a simple traceroute tool.

Each snippet is contained in its own file with comments explaining the code.

## Contributing
Contributions are welcome! If you have any suggestions, improvements, or new snippets, please create an issue or submit a pull request.

1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature-branch
   ```
3. Commit your changes:
   ```bash
   git commit -m "Description of your changes"
   ```
4. Push to the branch:
   ```bash
   git push origin feature-branch
   ```
5. Create a new pull request.

## Contact
For any questions or feedback, please contact:
- GitHub: [Ali-Ch-001](https://github.com/Ali-Ch-001)
