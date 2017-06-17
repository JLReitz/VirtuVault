#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <vector>

using namespace std;

#define PORTNO 1 //The port number for the server
const char * hostname = "q"; //The device name of the server


// Base class for the client-end and server-end socket connections **********************
public class Socket
{
protected:
	
	// Protected Fields
	
	int sockfd, portno;
    struct sockaddr_in serv_addr;
	
	//Protected Methods
	
	void disconnect() = 0;
	
public:
	
	// Public Methods
	bool connect(char * error) = 0;
	bool send(const char * message, char * error) = 0;
	bool receive(char * recieved, char * error) = 0;
}

// Client Socket Class ******************************************************************
public class Client_Socket : protected Socket
{
	Client_Socket();
	~Client_Socket();
	
protected:

	// Protected Fields
	
	struct hostent *server;
	
	//Protected Methods
	
	void disconnect();
	
public:

	// Public Methods
	
	bool connect(char * error);
	bool send(const char * message, char * error);
	bool receive(char * recieved, char * error);
	
}

// Server Socket Class ******************************************************************
public class Server_Socket : protected Socket
{
	Server_Socket();
	~Server_Socket();
	
protected:
	
	// Protected Fields
	
	int newsockfd;
	socklen_t clilen;
	
	struct sockaddr_in cli_addr;
	
	// Protected Methods
	
	void disconnect(char * evacuated);
	
public:

	// Public Methods
	
	bool connect(char * error);
	bool send(const char * message, char * error);
	bool receive(char * recieved, char * error);
}

#endif