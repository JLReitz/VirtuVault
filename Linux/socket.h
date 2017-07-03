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


//Struct for error reporting
typedef enum
{
	//Error Codes
	ERR_SOCK_NOHOST = 0,	//Hostname not found
	ERR_SOCK_OPEN = 1,		//Error opening socket
	ERR_SOCK_CONNECT = 2,	//Error connecting to socket
	ERR_SOCK_WRITE = 3,		//Error writing to socket
	ERR_SOCK_READ = 4,		//Error reading from socket
	
	ERR_VVAULT_HANDSHAKE = 5,	//Error completing communications handshake
	
	FLAG_VVAULT_COMPREEXIT = 6	//Exited the receive function before ACK_DONE was sent
} SystemCode;

// Base Socket Class ********************************************************************

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

// Client Socket Class ******************************************************************

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

// Server Socket Class ******************************************************************

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