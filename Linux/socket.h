#ifndef SOCKET_H
#define SOCKET_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


// Base Socket Class ***************************************************************************************************

class Socket
{
public:
	
	// Public Methods
	virtual bool send(const char * message, SystemCode & error) = 0;
	virtual bool receive(char * recieved, SystemCode & error) = 0;

protected:
	
	// Protected Fields
	int sockfd, portno;
    struct sockaddr_in serv_addr;
};

// Client Socket Class *************************************************************************************************

class Client_Socket : public Socket
{

public:

	Client_Socket();
	~Client_Socket();

	// Public Methods
	bool open(const char * hostname, SystemCode & error);
	bool send(const char * message, SystemCode & error);
	bool receive(char * const recieved, SystemCode & error);

protected:

	// Protected Fields
	struct hostent *server;
	
	//Protected Methods
	void disconnect();
};

// Server Socket Class *************************************************************************************************

class Server_Socket : public Socket
{	
public:

	Server_Socket();
	~Server_Socket();

	// Public Methods
	bool open(SystemCode & error);
	bool send(const char * message, SystemCode & error);
	bool receive(char * const recieved, SystemCode & error);

protected:
	
	// Protected Fields
	int newsockfd;
	socklen_t clilen;
	
	struct sockaddr_in cli_addr;
	
	// Protected Methods
	void disconnect(char * const evacuated);
};


#endif