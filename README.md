# webserv

**webserv** is a lightweight and efficient HTTP server implementation built from scratch.  
This project serves as both a learning tool and a foundation for building more complex web server applications.

## Features

- Supports HTTP/1.1 with basic methods (GET, POST, DELETE).
- Handles multiple client connections concurrently.
- Configurable server settings via a configuration file.
- Custom error pages for better user experience.
- Basic logging to track requests and errors.

## Installation

To include **webserv** in your project, clone the repository and compile the project:

```bash
    git clone git@github.com:shatilovdr/webserv.git
    cd webserv
    make
```

## Usage

To start the program run webserver command in terminal. (By default programm uses configuration conf/default.conf)
```bash
   ./webserver [configuration file]
```
## Configuration File

The **webserv** configuration file defines how the server behaves and handles incoming requests. It is structured into blocks that specify various directives for the server and its different locations.

### Server Block

- **listen**:  
  Specifies the IP address and port on which the server will listen.  
  _Example_: `127.0.0.1:8080` means the server listens on localhost at port 8080.

- **server_name**:  
  Sets the domain name for the server.  
  _Example_: `one.example.com` designates the hostname.

- **client_max_body_size**:  
  Limits the maximum allowed size of the client request body.  
  _Example_: `10M` restricts uploads and other request bodies to 10 megabytes.

- **error_page**:  
  Defines a custom error page for a specific HTTP status code.  
  _Example_: `error_page 404 /www/error_pages/404.html` means that when a 404 error occurs, the server will serve the custom page located at `/www/error_pages/404.html`.

### Location Blocks

Location blocks control how requests for specific paths are handled. Each block can have its own directives to manage access, file serving, and additional settings.

#### Location `/`

- **limit_except**:  
  Restricts the allowed HTTP methods. Only `GET`, `POST`, and `DELETE` are permitted.
  
- **root**:  
  Specifies the directory from which files will be served for this location.  
  _Example_: `root /www` means the server will look for files in the `/www` directory.

- **autoindex**:  
  When enabled (`on`), allows the server to generate a directory listing if no index file is found.

- **index**:  
  Sets the default file to serve when a directory is requested.  
  _Example_: `index index.html` designates `index.html` as the default file.

- **upload**:  
  Defines the directory where uploaded files are stored.  
  _Example_: `upload /uploads` directs file uploads to the `/uploads` folder.

#### Location `/uploads`

This block specifically handles requests for the `/uploads` path.

- **limit_except**:  
  Restricts allowed methods to `GET`, `POST`, and `DELETE`.

- **root**:  
  Specifies the base directory for this location.  
  _Example_: `root /uploads` indicates files are served from the `/uploads` directory.

- **autoindex**:  
  Enables directory listing if no default index file is found.

- **index**:  
  Uses `index.html` as the default file.

- **upload**:  
  Specifies that uploads for this location are also stored in the `/uploads` directory.

#### Location `/cgi`

This block is used for handling CGI (Common Gateway Interface) requests.

- **limit_except**:  
  Again, only `GET`, `POST`, and `DELETE` methods are allowed.

- **root**:  
  Points to the directory containing CGI scripts.  
  _Example_: `root /cgi-bin` means CGI scripts reside in the `/cgi-bin` directory.

- **autoindex**:  
  Enables directory listing if required.

- **index**:  
  Sets `index.html` as the default file.

- **upload**:  
  Even in the CGI context, uploads are directed to the `/uploads` directory.

---

This configuration file offers a flexible setup for managing static content, file uploads, and dynamic CGI scripts while enforcing security measures 
