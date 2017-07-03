#include "VirtuVault.h"


// VirtuVault ***************************************************************************

//Encrypts a string passed in through 'src' and returns the modified string through
//'dest'
//-	'dest' must be a mutable string type
void VirtuVault::encrypt(char * dest, const char * src)
{
	
}

//Decrypts a string passed in through 'src' and returns the modified string through
//'dest'
//-	'dest' must be a mutable string type
void VirtuVault::decrypt(char * dest, const char * src)
{
}

//Receives all messages from the sender through the socket
//-	Returns true if the messages were received correctly
//-	If an error occurs, the value returned will be false and an error code will be
//	provided through 'error'
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
	
	if(!socket->receieve(buffer, error)) //Wait for the "Acknowledge Done" code from the 
										 //sender
		return false;
	
	if(strcmp(buffer, ACK_DONE)) //If the sender hasn't sent ACK_DONE yet
		error = FLAG_VVAULT_COMPREEXIT;
}

//Sends all strings stored within 'messages' through the socket
//-	Will not clear 'messages' when finished. This is so that the data can be recovered
//	or saved should an error occur
//-	Returns true if the message[s] is sent successfully
//-	If an error occurs, the value returned will be false and an error code will be
//	provided through 'error'
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
//-	The number of messages that will be sent (to be included in this handshake) are
//	passed in as 'numMessages'
//-	Returns true if the handshake is successful
//-	If an error occurs, the value returned will be false and an error code will be
//	provided through 'error'
bool VirtuVault::handshake_send(int numMessages, SystemCode & error)
bool VirtuVault::handshake_send(int numMessages, SystemCode & error)
{		
	if(!socket->send(ACK, error)) //Send "Query for Acknowledgement" code
		return false;
	
	if(!socket->read(buffer, error))
		return false;
	
	if(!strcmp(buffer, ACK_READY)) //Continue with the handshake if the socket has received
								   //the "Acknowledged and Ready" code
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
		
	if(!strcmp(buffer, ACK)) //Continue with the handshake if the socket has received
							 //the "Query for Acknowledgement" code
	{
		//Continue with handshake
		if(!socket->send(ACK_READY, error)) //Send "Acknowledged and ready" code
			return -1;
			
		if(!socket->receive(buffer, error)) //Wait to receieve precursor info
											//(how many messages to expect)
			return -1;
		
		return (int)strtol(buffer, nullptr, 10); //Return the number value stored within 
												 //the string. This is the precursor info
	}
	else
	{
		error = ERR_VVAULT_HANDSHAKE;
		return -1;
	}
}

// Client_VirtuVault ********************************************************************

// Server_VirtuVault ********************************************************************

