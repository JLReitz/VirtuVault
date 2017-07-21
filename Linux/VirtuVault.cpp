#include "VirtuVault.h"


// VirtuVault **********************************************************************************************************

// Protected -----------------------------------------------------------------------------------------------------------

//Encrypts a string passed in through 'src' and returns the modified string through 'dest'
//-	'dest' must be a mutable string type
void VirtuVault::encrypt(char * dest, const char * src)
{
}

//Decrypts a string passed in through 'src' and returns the modified string through 'dest'
//-	'dest' must be a mutable string type
void VirtuVault::decrypt(char * dest, const char * src)
{
}

//Inserts an entry to the log file based on the System Code that is provided
//-	Reporting to the log file follows this format:
//		1. Date and time
//		2. Report the System Code followed by a description
void VirtuVault::log(const SystemCode & code)
{
	//Initialize fstream object and wrap the log file
	fstream log;
	log.open("VirtuVault_Operations_Log.txt", fstream::out | fstream::app);
	
	//Print a section divider and the date and time
	log << "******************************************************************" << endl;
	//TODO print date and time
	
	//Determine the error that occured and print it to the file
	switch(code)
	{
		case ERR_SOCK_NOHOST:
			log << "ERR_SOCK_NOHOST" << endl;
			log << "The hostname of the requested server could not be found" << endl;
			break;
		case ERR_SOCK_OPEN:
			log << "ERR_SOCK_OPEN" << endl;
			log << "The socket could not be opened for communications" << endl;
			break;
		case ERR_SOCK_CONNECT:
			log << "ERR_SOCK_CONNECT" << endl;
			log << "This socket iteration could not connect to the necessary service" << endl;
			break;
		case ERR_SOCK_WRITE:
			log << "ERR_SOCK_WRITE" << endl;
			log << "The program could not write to the socket" << endl;
			break;
		case ERR_SOCK_READ:
			log << "ERR_SOCK_READ" << endl;
			log << "The program could not read from the socket" << endl;
			break;
		case ERR_VVAULT_HANDSHAKE:
			log << "ERR_VVAULT_HANDSHAKE" << endl;
			log << "The program was unable to complete the necessary handshake to begin
					the transmission of information" << endl;
			break;
		case default:
			log << "An unknown error occured" << endl;
			break;
	}
}

//Runs a Cyclic Redundancy Check on the string included
//-	This is not a classic CRC, but some sort of check needed to be done, and this will suffice
void VirtuVault::CRC (char * crc, char * str)
{
	//Generate byte-sized variables to be used in CRC calculations
	unsigned char crc_compute = 0, crc_compare = 0b10010001;
	
	//Find the length of the string
	int strlen = strlen(str);
	
	//Iterate through the length of str and perform an 'xor' operation on each character
	for(int i=0; i<strlen; i++)
	{
		//'And' the current character with crc_compare
		unsigned char crc_compared = str[i] & crc_compare;
		
		//Update crc_compute by 'xor'ing it with the result
		crc_compute ^= crc_compared;
	}
	
	//Format the resulting crc binary value into a string
	//-	It is safe to use snprintf() because we know and expect a certain maximum size to each message. These are not 
	//	dynamically allocated and the length of each string is taken into account when checking against the CRC
	snprintf(crc, 256, "%d", (unsigned int)crc_compute);
}

//TODO: implement CRC
//Receives all messages from the sender through the socket
//-	Returns true if the messages were received correctly
//-	If an error occurs, the value returned will be false and an error code will be provided through 'error'
bool VirtuVault::receiveMessage(SystemCode & error)
{
	int numMessages = handshake_receieve(error); //Handshake
	
	if(numMessages < 0)
		return false;
	
	//Reserve space in 'data' for the new messages
	int listSize = receive.size(); //If there's any messages in data already, this will
								   //account for that
	receive.reserve(listSize + numMessages);
	
	//Begin receiving messages
	for(int i=0; i<numMessages; i++)
	{
		if(!socket->send(ACK_READY, error)) //Send "Acknowledged and Ready" code
			return false;
		
		//Receieve the next message
		if(!socket->receieve(buffer, error)
			return false;
		
		if(strcmp(buffer, ACK_DONE)) //If the sender acknowledges it is done sending
			return true; //End the function earlier than expected
		
		//Copy the new message into 'data'
		receive.add();
		strcpy(receive[listSize+i], buffer);
	}
	
	if(!socket->receieve(buffer, error)) //Wait for the "Acknowledge Done" code from the sender
		return false;
	
	if(strcmp(buffer, ACK_DONE)) //If the sender hasn't sent ACK_DONE yet
		error = FLAG_VVAULT_COMPREEXIT;
}

//TODO: implement CRC
//Sends all strings stored within 'messages' through the socket
//-	Will not clear 'messages' when finished. This is so that the data can be recovered or saved should an error occur
//-	Returns true if the message[s] is sent successfully
//-	If an error occurs, the value returned will be false and an error code will be provided through 'error'
bool VirtuVault::sendMessage(SystemCode & error)
{
	int numMessages = send.size();
	
	if(!handshake_send(numMessages, error)) //Handshake
		return false;
		
	//Send messages
	for(i=0; i<numMessages; i++)
	{	
		//Wait for the recipient to send the "Acknowledged and Ready" code
		do
		{	
			if(!socket->receive(buffer, error))
				return false
	
		} while(strcmp(buffer, ACK_READY));
	
		//Send the next message in the list	
		if(!socket->send(send[i], error))
			return false;
	}
	
	if(!socket->send(ACK_DONE, error)) //Send "Acknowledge Done" code
		return false;
		
	return true;
}

//The handshake authentication that must occur on the sending-end of a message
//before the message is sent
//-	The number of messages that will be sent (to be included in this handshake) are passed in as 'numMessages'
//-	Returns true if the handshake is successful
//-	If an error occurs, the value returned will be false and an error code will be provided through 'error'
bool VirtuVault::handshake_send(int numMessages, SystemCode & error)
{		
	if(!socket->send(ACK, error)) //Send "Query for Acknowledgement" code
		return false;
	
	if(!socket->read(buffer, error))
		return false;
	
	if(!strcmp(buffer, ACK_READY)) //Continue with the handshake if the socket has received the "Acknowledged and Ready" code
	{
		//Continue with handshake
		
		//Create the precursor string
		char precursorInfo[256];
		sprintf(str, "%d", numMessages); 	
		
		//Send the precursor string
		if(!socket->send(precursorInfo, error))
			return false;
		
		return true; //Return true when the handshake is complete
	}
	else
	{
		error = ERR_VVAULT_HANDSHAKE;
		return false;
	}
}

//The handshake authentication that must occur on the recieving-end of a message
//before the message is sent
//-	Returns the number of messages that should be expected
//-	If an error occurs, the value returned will be -1 and an error code will be provided
//	through 'error'
int VirtuVault::handshake_receive(SystemCode & error)
{
	bool success = false;
	
	if(!socket->receive(buffer, error)) //Read from the socket to buffer
		return -1;
		
	if(!strcmp(buffer, ACK)) //Continue with the handshake if the socket has received the "Query for Acknowledgement" code
	{
		//Continue with handshake
		if(!socket->send(ACK_READY, error)) //Send "Acknowledged and ready" code
			return -1;
			
		if(!socket->receive(buffer, error)) //Wait to receieve precursor info (how many messages to expect)
			return -1;
		
		return (int)strtol(buffer, nullptr, 10); //Return the number value stored within  the string. This is the precursor info
	}
	else
	{
		error = ERR_VVAULT_HANDSHAKE;
		return -1;
	}
}

unsigned long VirtuVault::get_encKey() const
{
	return this->encKey;
}

void VirtuVault::set_encKey(const long new_encKey)
{
	this->encKey = new_encKey;
}

// Client_VirtuVault ***************************************************************************************************

Client_VirtuVault::Client_VirtuVault()
{
	//Run startup()
}

Client_VirtuVault::~Client_VirtuVault()
{
	//Free allocated Socket
	delete this->socket;
}

// Public --------------------------------------------------------------------------------------------------------------

bool Client_VirtuVault::run_Process(char * topic, char * options, SystemCode & error)
{
	return false;
}

// Protected ----------------------------------------------------------------------------

void Client_VirtuVault::load_returned()
{
}

bool Client_VirtuVault::connectSocket(SystemCode & error)
{
	return false;
}

bool Client_VirtuVault::connectSocket(Systems & error)
{
	return false;
}

bool Client_VirtuVault::setup(SystemCode & error)
{
	return false;
}

bool Client_VirtuVault::startup(SystemCode & error)
{
	return false;
}

// Server_VirtuVault ***************************************************************************************************

Server_VirtuVault::Server_VirtuVault()
{
	//Run startup()
}

Server_VirtuVault::~Server_VirtuVault()
{
	//Free allocated Socket
	delete this->socket;
}


// Public --------------------------------------------------------------------------------------------------------------

bool Server_VirtuVault::run_Process(SystemCode & error)
{
	return false;
}

// Protected -----------------------------------------------------------------------------------------------------------

bool Server_VirtuVault::setup(SystemCode & error)
{
	return false;
}

bool Server_VirtuVault::startup(SystemCode & error)
{
	return false;
}
