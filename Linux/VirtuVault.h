#ifndef VIRTUVAULT_H
#define VIRTUVAULT_H


#include "Socket.h"


//Acknowledge codes for socket communication
const char * ACK = "ACK\0", ACK_READY = "ACK_READY\0", ACK_DONE = "ACK_DONE\0";

// Base VirtuVault class ***********************************************************************************************

class VirtuVault
{
public:
	
	//Public Functions	
	virtual bool connect(SystemCode & error);
	
	
protected:

	//A struct designed to contain information on passwords/files returned by the query
	//-	'name' is the name of the file or account for which the password exists
	//-	'description' is the description of the file or the password for the account
	typedef struct
	{
		char * name, description; 
	} VVault_Object;
	
	char buffer[256]; //The buffer to store strings read from the socket into temporarily
	vector <char[256]> receive, send; //A vector to store received data and messages to be
								   //sent
	
	Socket * socket; //The Socket object that is oh-so essential to this operation
	
	//Protected Functions
	void encrypt(char * dest, const char * src);
	void decrypt(char * dest, const char * src);
	void log(const SystemCode & error);
	void CRC (char * crc, char * str);
	bool receiveMessage(SystemCode & error);
	bool sendMessage(SystemCode & error);
	bool handshake_send(int numMessages, char * error);
	int handshake_recieve(SystemCode & error);
	
	virtual bool setup(SystemCode & error) = 0;
	virtual bool startup(SystemCode & error) = 0;
	
	//Protected Accessors
	unsigned long get_encKey() const;
	
	//Protected Mutators
	void set_encKey(const long new_encKey);
	
private:

	//Private Fields
	unsigned long encKey; //The encyrption key to be used in encrypting/decrypting messages

};

// Client VirtuVault class *********************************************************************************************

class Client_VirtuVault : public VirtuVault
{
public:

	Client_VirtuVault();
	~Client_VirtuVault();
	
	//Public Functions
	bool run_Process(char * topic, char * options, SystemCode & error);

protected:
	
	//Protected Members
	char * hostname; //The name of the host server (necessary to connect)
	
	vector <VVault_Object> results; //A vector for all of the query results to be stored in a less abstract format
	
	//Protected Functions
	void load_returned();
	bool connectSocket(SystemCode & error);
	bool setup(SystemCode & error);
	bool startup(SystemCode & error);
};

// Server VirtuVault class *********************************************************************************************

class Server_VirtuVault : public VirtuVault
{
public:

	Server_VirtuVault();
	~Server_VirtuVault();
	
	//Public Functions
	bool run_Process(SystemCode & error);

protected:

	//Protected Fields
	

	//Protected Functions
	bool setup(SystemCode & error);
	bool startup(SystemCode & error);

};


#endif