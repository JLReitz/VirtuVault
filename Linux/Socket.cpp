#include "Socket.h"


// Client_Socket ************************************************************************

// Public -------------------------------------------------------------------------------

Client_Socket::Client_Socket()
{
	this->sockfd = -1;
	this->portno = PORTNO;
	
	//Set complex data types to zero
	this->server = nullptr;
	bzero((char *)&(this->serv_addr), sizeof(this->serv_addr));
}

Client_Socket::~Client_Socket()
{
	this->disconnect();
	
	//Free pointers
	delete this->server;
}

bool Client_Socket::open(const char * hostname, SystemCode & error)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if(sockfd < 0)
	{
		error = ERR_SOCK_OPEN;
		return false;
    }
	
    server = gethostbyname(hostname);
	
    if(!server) 
	{
        error = ERR_SOCK_NOHOST;
        return false;
    }
	
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
	
    if(connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
        error = ERR_SOCK_CONNECT;
		return false;
	}
	
	return true;
}

//Sends a characer string as a message through the socket
//-	This character string is 'message'
//-	'message' must be of size 256
//- A terminary character will be concatinated to the message as it is recieved
//	on the other side
//-	If an error occurs, this function will return false and an error message will be
//	be available through 'error'
bool Client_Socket::send(const char * message, SystemCode & error)
{
	//Write message to socket
    int n = write(sockfd, message, strlen(message));
	
    if(n < 0) 
	{
		error = ERR_SOCK_WRITE;
		return false;
	}
	
	return true;
}

//Reads data from the socket into a character array
//-	This character array is 'recieved'
//-	'recieved' must be of size 256
//-	'recieved' does not need to be cleared before calling this function
//-	A terminary character will be concatinated to the end of the string before it is
//	returned
//-	If an error occurs, this function will return false and an error message will be
//	be available through 'error'
bool Client_Socket::receive(char * recieved, SystemCode & error)
{
	//Clear recieved
	bzero(recieved, 256);
	
	//Read from socket to recieved
    int n = read(sockfd, recieved, 255); //Leave one character open for terminary
    									 //character
	
	if(n < 0)
	{
		error = ERR_SOCK_READ;
		return false;
	}
	
	//Concatinate a terminary character to the end of the recueved string
	*(recieved+n+1) = '\0';
	
	return true;
}

// Protected Methods --------------------------------------------------------------------

void Client_Socket::disconnect()
{
	//Close the socket
	close(sockfd);
}

// Server_Socket ************************************************************************

// Public -------------------------------------------------------------------------------

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

Server_Socket::~Server_Socket()
{
	char evacuated[256];
	
	bzero(evacuated, 256);
	
	this->disconnect(evacuated);
}

bool Server_Socket::open(SystemCode & error)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 
    if(sockfd < 0)
	{
		*error = ERR_SOCK_OPEN;
		return false;
    }
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	 
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		error = 'b'; //Error binding to socket
		return false;
	}
		  
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	return true;
}

//Sends a characer string as a message through the socket
//-	This character string is 'message'
//-	'message' must be of size 256
//- A terminary character will be concatinated to the message as it is recieved
//	on the other side
//-	If an error occurs, this function will return false and an error message will be
//	be available through 'error'
bool Server_Socket::send(const char * message, SystemCode & error)
{
	//Write message to socket
    int n = write(newsockfd, message, strlen(message));
	
    if(n < 0) 
	{
		error = ERR_SOCK_WRITE
		return false;
	}
	
	return true;
}

//Reads data from the socket into a character array
//-	This character array is 'recieved'
//-	'recieved' must be of size 256
//-	'recieved' does not need to be cleared before calling this function
//-	A terminary character will be concatinated to the end of the string before it is
//	returned
//-	If an error occurs, this function will return false and an error message will be
//	be available through 'error'
bool Server_Socket::receive(char * const recieved, char * const error)
{
	//Clear recieved
	bzero(recieved, 256);
	
	//Read from the socket
	int n = read(newsockfd, recieved, 255); //Leave one character open for terminary
											//character
	
	if(n < 0)
	{
		error = ERR_SOCK_READ;
		return false;
	}
	
	//Concatinate a terminary character to the end of the recueved string
	*(recieved+n+1) = '\0';
	
	return true;
}

// Protected Methods --------------------------------------------------------------------

//Disconnects from the socket on the server side
//Will read and return any leftover data contained within the socket as well
//-	This data is returned through evacuated
//-	evacuated must be a char array of size 256
//-	evacuated does not need to be cleared before calling this function
void Server_Socket::disconnect(char * const evacuated)
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
