# WebServ

WebServ is a simple web server implemented in C++. It supports basic HTTP functionalities including GET, POST, and DELETE requests, directory listing, and CGI execution.

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/webserv.git
    cd webserv
    ```

2. Build the project:
    ```sh
    make
    ```

## Usage

To start the server, run the following command:
```sh
./webserv [configuration file]
```

If no configuration file is provided, the server will use the default configuration.

## Configuration
Configuration files are located in the conf/ directory. You can specify a custom configuration file when starting the server.

## Features
- GET Requests: Serve static files and directory listings.
- POST Requests: Handle form submissions and file uploads.
- DELETE Requests: Delete files from the server.
- CGI Support: Execute CGI scripts for dynamic content.
