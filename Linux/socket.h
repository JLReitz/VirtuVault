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

// Base class for the client-end and server-end socket connections ------------------------------------------------------------
public class Socket
{
	Socket(const int new_sockfd = 0, const int new_portno = 0, const int new_n = 0);
	virtual ~Socket();
	
	private:
	
	// Private Fields
	
	int sockfd, portno, n, dataRecieved;
	
	char buffer[256];
	
    struct sockaddr_in serv_addr;
    struct hostent *server;
	
	vector <char *> data;
	
	// Private Methods
	
	bool connect();
	bool disconnect();
	int update_dataRecieved();
	
	public:
	
	// Accessors
	
	const int get_dataRecieved();
	const char * get_Data();
	
	// Public Methods
	
	bool query(const char *message);
	bool receive();
}

// Client Socket Class --------------------------------------------------------------------------------------------------------
public class Client_Socket : Socket
{
	public:
	
	private:
}

// Server Socket Class --------------------------------------------------------------------------------------------------------
public class Server_Socket : Socket
{
	public:
	
	private:
}
