#ifndef VIRTUVAULT_H
#define VIRTUVAULT_H


#include <vector>
#include <iostream>
#include <fstream>
#include "Socket.h"
#include "VVault_Security.h"


// Base VirtuVault class ***********************************************************************************************

class VirtuVault
{
public:
	
	//Public Functions	
	virtual SYS_CODE_T connect(ERR_CODE_T & error) = 0;
	
	
protected:

	//Protected Members
	
	//A struct designed to contain information on passwords/files returned by the query
	//-	'name' is the name of the file or account for which the password exists
	//-	'description' is the description of the file or the password for the account
	typedef struct
	{
		char * name, description; 
	} VVault_Object;
	
	BYTE buffer[256]; //The buffer to store strings read from the socket into temporarily
	vector <string> receive, send; //A vector to store received data and messages to be sent
	STATE_HANDSHAKE_T curr_handshake_state, prev_handshake_state;
	
	Socket * socket; //The Socket object that is oh-so essential to this operation
	
	//Protected Functions
	SYS_CODE_T receiveMessage(ERR_CODE_T & error);
	SYS_CODE_T sendMessage(ERR_CODE_T & error);
	SYS_CODE_T handshake_send(const int numMessages, ERR_CODE_T & error);
	SYS_CODE_T handshake_recieve(int & numMessages, ERR_CODE_T & error);
	
	virtual SYS_CODE_T setup(ERR_CODE_T & error) = 0;
	virtual SYS_CODE_T startup(ERR_CODE_T & error) = 0;
	
};

// Client VirtuVault class *********************************************************************************************

class Client_VirtuVault : public VirtuVault
{
public:

	Client_VirtuVault()
	{
		//Run startup()
	}
	
	~Client_VirtuVault()
	{
		//Free allocated Socket
		delete this->socket;
	}
	
	//Public Functions
	SYS_CODE_T connect(ERR_CODE_T & error);
	SYS_CODE_T run_Process(const char * topic, const char * options, ERR_CODE_T & error);
	
protected:
	
	//Protected Members
	char * hostname; //The name of the host server (necessary to connect)
	
	vector <VVault_Object> results; //A vector for all of the query results to be stored in a less abstract format
	
	//Protected Functions
	void load_returned();
	SYS_CODE_T setup(ERR_CODE_T & error);
	SYS_CODE_T startup(ERR_CODE_T & error);
	
};

// Server VirtuVault class *********************************************************************************************

class Server_VirtuVault : public VirtuVault
{
public:

	Server_VirtuVault()
	{
		//Run startup()
	}
	
	~Server_VirtuVault()
	{
		//Free allocated Socket
		delete this->socket;
	}
	
	//Public Functions
	SYS_CODE_T connect(ERR_CODE_T & error);
	SYS_CODE_T run_Process(ERR_CODE_T & error);

protected:

	//Protected Members
	vector <VVault_Object> results; //A vector for all of the query results to be stored in a less abstract format

	//Protected Functions
	SYS_CODE_T setup(ERR_CODE_T & error);
	SYS_CODE_T startup(ERR_CODE_T & error);
	
};


#endif