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
#include <vector>

using namespace std;

#define PORTNO 51717 //The port number for the server


// Base class for the client-end and server-end socket connections **********************

class Socket
{
public:
	
	// Public Methods
	virtual bool send(char * message, char * error) = 0;
	virtual bool receive(char * recieved, char * error) = 0;

protected:
	
	// Protected Fields
	int sockfd, portno;
    struct sockaddr_in serv_addr;
};

// Client Socket Class ******************************************************************

class Client_Socket : public Socket
{

public:

	Client_Socket();
	~Client_Socket();

	// Public Methods
	bool open(char * hostname, char * error);
	bool send(char * message, char * error);
	bool receive(char * recieved, char * error);

protected:

	// Protected Fields
	struct hostent *server;
	
	//Protected Methods
	void disconnect();
};

// Server Socket Class ******************************************************************

class Server_Socket : public Socket
{	
public:

	Server_Socket();
	~Server_Socket();

	// Public Methods
	bool open(char * error);
	bool send(char * message, char * error);
	bool receive(char * recieved, char * error);

protected:
	
	// Protected Fields
	int newsockfd;
	socklen_t clilen;
	
	struct sockaddr_in cli_addr;
	
	// Protected Methods
	void disconnect(char * evacuated);
};

#endif
