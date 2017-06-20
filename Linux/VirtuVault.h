#include "Socket.h"


// Base VirtuVault class ****************************************************************

class VirtuVault
{
public:
	
protected:

	//Protected Fields
	char buffer[256];
	vector <char *> data, messages;
	
	Socket *socket;
	
	//Protected Functions
	bool receiveMessage(char *error);
	bool sendMessage(char * error);
	bool handshake_send(int numMessages, char * error);
	int handshake_recieve(char * error);
	virtual bool setup(char * error) = 0;
	virtual bool startup(char * error) = 0;

};

// Client VirtuVault class **************************************************************

class Client_VirtuVault : public VirtuVault
{
public:

	Client_VirtuVault();
	~Client_VirtuVault();
	
	//Public Functions
	bool connectSocket(char * error);
	bool query(char * query, char * options, char * error);

protected:
	
	//Protected Fields
	char * hostname;
	
	//Protected Functions
	bool setup(char * error);
	bool startup(char * error);
};

// Server VirtuVault class **************************************************************

class Server_VirtuVault : public VirtuVault
{
public:

protected:

	//Protected Functions
	bool setup(char * error);
	bool startup(char * error);

};
