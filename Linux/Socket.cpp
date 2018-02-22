#include "Socket.h"


// Client_Socket *******************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

//Establishes a socket connection between the local device and an internet-connected host
//
//	[Input}:	hostname -	A string pointer to the name of the host device
//	[Output]:				An error code (will return ERR_OK if no error occured)
CODE_ERROR_T Client_Socket::connect(const char * const hostname)
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if(m_sockfd < 0)
		return ERR_SOCK_OPEN;
	
    m_server = gethostbyname(hostname);
	
    if(!m_server) 
		return ERR_SOCK_NOHOST;
	
    bzero((char *) &m_serv_addr, sizeof(m_serv_addr));
    m_serv_addr.sin_family = AF_INET;
	
    bcopy((char *)m_server->h_addr, (char *)&m_serv_addr.sin_addr.s_addr, m_server->h_length);
    m_serv_addr.sin_port = htons(m_portno);
	
    if(connect(m_sockfd,(struct sockaddr *) &m_serv_addr, sizeof(m_serv_addr)) < 0)
		return ERR_SOCK_CONNECT;
	
	return ERR_NONE;
}

//Sends a buffer of Bytes through the socket
//- A terminary character will be concatinated to the message as it is recieved on the other side
//-	If an error occurs, this function will return false and an error message will be be available through 'error'
//
//	[Input]:	message 	-	A pointer to the buffer where the message is contained.
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 256)
//	[Output]:					An error code (will return ERR_NONE if no error occured)
CODE_ERROR_T Client_Socket::send(BYTE * const message, const int messageSize)
{
	//Write message to socket
    int n = write(m_sockfd, message, messageSize);
	
    if(n < 0 || n != messageSize) 
		return ERR_SOCK_WRITE;
	
	return ERR_NONE;
}

//Reads data from the socket into a BYTE array
//-	A terminary character will be concatinated to the end of the string before it is returned
//-	If an error occurs, this function will return false and an error message will be available through 'error'
//
//	[Input]:	received 	-	A pointer to the buffer for the receieved message to be placed
//								-	The buffer will be cleared within this function.
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 255)
//	[Output]:					An error code (will return ERR_NONE if no error occured)
CODE_ERROR_T Client_Socket::receive(BYTE * const recieved, const int messageSize)
{
	//Clear recieved
	bzero(recieved, messageSize);
	
	//Read from socket to recieved
    int n = read(m_sockfd, recieved, messageSize); //Leave one character open for terminary character
	
	if(n < 0)
		return ERR_SOCK_READ;
	
	//Concatinate a terminary character to the end of the recueved string
	*(recieved+n+1) = '\0';
	
	return ERR_NONE;
}

// Protected Methods ---------------------------------------------------------------------------------------------------

void Client_Socket::disconnect()
{
	//Close the socket
	close(sockfd);
}


// Server_Socket *******************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

//Establishes a socket connection between the local device and an internet-connected host
//
//	[Output]:	An error code (will return ERR_OK if no error occured)
CODE_ERROR_T Server_Socket::connect()
{
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 
    if(m_sockfd < 0)
		return ERR_SOCK_OPEN;
	
	m_serv_addr.sin_family = AF_INET;
	m_serv_addr.sin_addr.s_addr = INADDR_ANY;
	m_serv_addr.sin_port = htons(m_portno);
	 
	if(bind(m_sockfd, (struct sockaddr *) &m_serv_addr, sizeof(m_serv_addr)) < 0) 
		return ERR_SOCK_BIND;
		  
	listen(sockfd,5);
	m_clilen = sizeof(cli_addr);

	m_newsockfd = accept(m_sockfd, (struct sockaddr *) &m_cli_addr, &m_clilen);
	
	return ERR_NONE;
}

//Sends a buffer of BYTEs through the socket
//- A terminary character will be concatinated to the message as it is recieved on the other side
//-	If an error occurs, this function will return false and an error message will be be available through 'error'
//
//	[Input]:	message 	-	A pointer to the buffer where the message is contained.
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 255)
//	[Output]:					An error code (will return ERR_NONE if no error occured)
CODE_ERROR_T Server_Socket::send(BYTE * const message, const int messageSize)
{
	//Write message to socket
    int n = write(m_newsockfd, message, messageSize);
	
    if(n < 0 || n != messageSize)
		return ERR_SOCK_WRITE;
	
	return ERR_NONE;
}

//Reads data from the socket into a BYTE array
//-	A terminary character will be concatinated to the end of the string before it is returned
//-	If an error occurs, this function will return false and an error message will be available through 'error'
//
//	[Input]:	received 	-	A pointer to the buffer for the receieved message to be placed
//								-	The buffer will be cleared within this function
//	[Output]:	messageSize -	The size in Bytes of the message to be sent (defaults to 255)
//	[Output]:					An error code (will return ERR_NONE if no error occured)
CODE_ERROR_T Server_Socket::receive(BYTE * const recieved, const int messageSize)
{
	//Clear recieved
	bzero(recieved, messageSize);
	
	//Read from the socket
	int n = read(m_newsockfd, recieved, messageSize); //Leave one character open for terminary character
	
	if(n < 0)
		return ERR_SOCK_READ;
	
	//Concatinate a terminary character to the end of the received string
	*(recieved+n+1) = '\0';
	
	return ERR_NONE;
}

// Protected Methods --------------------------------------------------------------------

//Disconnects from the socket on the server side
//-	Will read and return any data still within the socket upon this function call
//-	'evacuated' must be of appropriate size; overflow detection is not present within this
//	function
//
//	[Output]:	evacuated -	A pointer to the buffer where the leftover data will be stored
//							-	The buffer will be cleared within this function
void Server_Socket::disconnect(BYTE * const evacuated)
{	
	int n = -1, i = 0; //TODO: Change this to something that works
	
	if(evacuated)
	{
		//Clear evacuated
		bzero(evacuated, 256);
		
		//Wait for there to be zero traffic on the socket
		do
		{
			//Evactuate the remaining information from the socket
			n = read(m_newsockfd, (evacuated+i), 1);
			i++;
			
		} while(n > 0);
	}
	
	//Close the socket
	close(m_newsockfd);
	close(m_sockfd);
}
