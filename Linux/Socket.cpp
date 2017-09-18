#include "Socket.h"


// Client_Socket *******************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

//Opens a socket between the local device and the host (server)
//
//	[Input}:	hostname -	A string pointer to the name of the host device
//	[Output]:				An error code (will return ERR_OK if no error occured)
CODE_ERROR_T Client_Socket::open(const char * hostname)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if(sockfd < 0)
		return ERR_SOCK_OPEN;
	
    server = gethostbyname(hostname);
	
    if(!server) 
		return ERR_SOCK_NOHOST;
	
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
	
    if(connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		return ERR_SOCK_CONNECT;
	
	return ERR_OK;
}

//Sends a characer string as a message through the socket
//- A terminary character will be concatinated to the message as it is recieved on the other side
//-	If an error occurs, this function will return false and an error message will be be available through 'error'
//
//	[Input]:	message 	-	A pointer to the buffer where the message is contained.
//								-	The message size must be a maximum size of 255 bytes.
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 255)
//	[Output]:					An error code (will return ERR_OK if no error occured)
CODE_ERROR_T Client_Socket::send(const void * message, const int messageSize)
{
	//Write message to socket
    int n = write(sockfd, message, messageSize);
	
    if(n < 0 || n != messageSize) 
		return ERR_SOCK_WRITE;
	
	return ERR_OK;
}

//Reads data from the socket into a character array
//-	A terminary character will be concatinated to the end of the string before it is returned
//-	If an error occurs, this function will return false and an error message will be available through 'error'
//
//	[Input]:	received 	-	A pointer to the buffer for the receieved message to be placed
//								-	The buffer must be a minimum size of 256 bytes.
//								-	The buffer will be cleared within this function.
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 255)
//	[Output]:					An error code (will return ERR_OK if no error occured)
CODE_ERROR_T Client_Socket::receive(const void * recieved, const int messageSize)
{
	//Clear recieved
	bzero(recieved, 256);
	
	//Read from socket to recieved
    int n = read(sockfd, recieved, messageSize); //Leave one character open for terminary character
	
	if(n < 0)
		return ERR_SOCK_READ;
	//Concatinate a terminary character to the end of the recueved string
	*(recieved+n+1) = '\0';
	
	return ERR_OK;
}

// Protected Methods ---------------------------------------------------------------------------------------------------

void Client_Socket::disconnect()
{
	//Close the socket
	close(sockfd);
}


// Server_Socket *******************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

//Opens a socket connection between the local device and the client
CODE_ERROR_T Server_Socket::open()
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 
    if(sockfd < 0)
		return ERR_SOCK_OPEN;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	 
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		return ERR_SOCK_BIND;
		  
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	return ERR_OK;
}

//Sends a characer string as a message through the socket
//- A terminary character will be concatinated to the message as it is recieved on the other side
//-	If an error occurs, this function will return false and an error message will be be available through 'error'
//
//	[Input]:	message 	-	A pointer to the buffer where the message is contained.
//								-	The message size must be a maximum size of 255 bytes.
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 255)
//	[Output]:					An error code (will return ERR_OK if no error occured)
CODE_ERROR_T Server_Socket::send(const void * message, const int messageSize)
{
	//Write message to socket
    int n = write(newsockfd, message, messageSize);
	
    if(n < 0 || n != messageSize)
		return ERR_SOCK_WRITE;
	
	return ERR_OK;
}

//Reads data from the socket into a character array
//-	A terminary character will be concatinated to the end of the string before it is returned
//-	If an error occurs, this function will return false and an error message will be available through 'error'
//
//	[Input]:	received 	-	A pointer to the buffer for the receieved message to be placed
//								-	The buffer must be a minimum size of 256 bytes.
//								-	The buffer will be cleared within this function.
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 255)
//	[Output]:					An error code (will return ERR_OK if no error occured)
CODE_ERROR_T Server_Socket::receive(const void * recieved, const int messageSize)
{
	//Clear recieved
	bzero(recieved, 256);
	
	//Read from the socket
	int n = read(newsockfd, recieved, messageSize); //Leave one character open for terminary character
	
	if(n < 0)
		return ERR_SOCK_READ;
	
	//Concatinate a terminary character to the end of the recueved string
	*(recieved+n+1) = '\0';
	
	return ERR_OK;
}

// Protected Methods --------------------------------------------------------------------

//Disconnects from the socket on the server side
//-	Will read and return any leftover data contained within the socket as well
//
//	[Output]:	evacuated -	A pointer to the buffer where the leftover data will be stored
//							-	The buffer must be a minimum size of 0
//							-	The buffer will be cleared within this function
void Server_Socket::disconnect(const void * evacuated)
{	
	int n = -1; //TODO: Change this to something that works

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
