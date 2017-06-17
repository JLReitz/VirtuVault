#include "Socket.h"


// Client Socket ************************************************************************

Client_Socket::Client_Socket()
{
	this->sockfd = -1;
	this->portno = PORTNO;
	
	//Set complex data types to zero
	this->server = nullptr;
	bzero((char *)&(this->serv_addr), sizeof(this->serv_addr));
	
	this->data.clear();
	this->data.reserve(1); //Set data to only hold one message at a time by default
}

Client_Socket::~Client_Socket()
{
	this->disconnect();
	
	//Free pointers
	delete this->server;
}

// Protected Methods --------------------------------------------------------------------

Client_Socket::void disconnect()
{
	//Close the socket
	close(sockfd);
}

// Public Methods -----------------------------------------------------------------------

Client_Socket::bool connect(char * error)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if(sockfd < 0)
	{
		*error = 'o'; //Error opening socket
		return false;
    }
	
    server = gethostbyname(hostname);
	
    if(!server) 
	{
        *error = 'h'; //No such host
        return false;
    }
	
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
	
    if(connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
        *error = 'c'; //Error connecting to socket
		return false;
	}
	
	return true;
}

//Sends a characer string as a message through the socket
//-	This character string is 'message'
//-	'message' must be of size 256
//-	If an error occurs, this function will return false and an error message will be
//	be available
Client_Socket::bool send(const char * message, char * error)
{
	//Write message to socket
    int n = write(sockfd, message, strlen(message));
	
    if(n < 0) 
	{
		*error = 'w'; //Error writing to socket
		return false;
	}
	
	return true;
}

//Reads data from the socket into a character string
//-	This character string is 'recieved'
//-	'recieved' must be of size 256
//-	'recieved' does not need to be cleared before calling this function
//-	If an error occurs, this function will return false and an error message will be
//	be available
Client_Socket::bool recieve(char * recieved, char * error)
{
	//Clear recieved
	bzero(recieved, 256);
	
	//Read from socket to recieved
    int n = read(sockfd, recieved, 255);
	
	if(n < 0)
	{
		*error = 'r'; //Error reading from socket
		return false;
	}
	
	return true;
}

// Server Socket ************************************************************************

Server_Socket::Server_Socket()
{
	this->sockfd = -1;
	this->newsockfd = -1;
	this->portno = PORTNO;
	this->clilen = -1;
	
	//Set complex data types to zero
	bzero((char *)&(this->serv_addr), sizeof(this->serv_addr));
	bzero((char *)&(this->cli_addr), sizeof(this->cli_addr));
}

// Protected Methods --------------------------------------------------------------------

//Disconnects from the socket on the server side
//Will read and return any leftover data contained within the socket as well
//-	This data is returned through evacuated
//-	evacuated must be a char array of size 256
//-	evacuated does not need to be cleared before calling this function
Server_Socket::void disconnect(char * evacuated)
{	
	int n = -1;

	//Clear evacuated
	bzero(evacuated, 256);
	
	//Wait for there to be zero traffic on the socket
	while(n > 0)
	{
		//Evactuate the remaining information from the socket
		n = read(newsockfd, evacuated, 255);
	}
	
	//Close the socket
	close(newsockfd);
	close(sockfd);
}

// Public Methods -----------------------------------------------------------------------

Server_Socket::bool connect(char * error)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 
    if(sockfd < 0)
	{
		*error = 'o'; //Error opening socket
		return false;
    }
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	 
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		*error = 'b'; //Error binding to socket
		return false;
	}
		  
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	return true;
}

//TODO
//Sends a characer string as a message through the socket
//-	This character string is 'message'
//-	'message' must be of size 256
//-	If an error occurs, this function will return false and an error message will be
//	be available
Server_Socket::bool send(const char * message, char * error)
{
	//Write message to socket
    int n = write(newsockfd, message, strlen(message));
	
    if(n < 0) 
	{
		*error = 'w'; //Error writing to socket
		return false;
	}
	
	return true;
}

//Reads data from the socket into a character string
//-	This character string is 'recieved'
//-	'recieved' must be of size 256
//-	'recieved' does not need to be cleared before calling this function
//-	If an error occurs, this function will return false and an error message will be
//	be available
Server_Socket::bool recieve(char * recieved, char * error);
{
	//Clear recieved
	bzero(recieved, 256);
	
	//Read from the socket
	int n = read(newsockfd, recieved, 255);
	
	if(n < 0)
	{
		*error = 'r'; //Error reading from socket
		return false;
	}
	
	return true;
}
