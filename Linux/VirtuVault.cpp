#include "VirtuVault.h"


// VirtuVault ***************************************************************************

//Receives all messages from the sender through the socket
//-	Returns true if the messages were received correctly
//-	If an error occurs, the value returned will be false and an error code will be
//	provided through 'error'
bool VirtuVault::receiveMessage(char * error)
{
	int numMessages = handshake_receieve(error); //Handshake
	
	if(numMessages < 0)
		return false;
	
	//Reserve space in 'data' for the new messages
	int dataSize = data.size(); //If there's any messages in data already, this will
								//account for that
	data.reserve(dataSize + numMessages);
	
	//Begin receiving messages
	for(int i=0; i<numMessages; i++)
	{
		if(!socket->send("ARdy", error)) //Send "Acknowledged and Ready" code
			return false;
		
		//Receieve the next message
		if(!socket->receieve(buffer, error)
			return false;
		
		if(strcmp(buffer, "ADon")) //If the sender acknowledges it is done sending
			return true; //End the function earlier than expected
		
		//Copy the new message into 'data'
		data.add();
		strcpy(data[datasize+i], buffer);
	}
	
	if(!socket->receieve(buffer, error)) //Wait for the "Acknowledge Done" code from the 
										 //sender
		return false;
	
	if(!strcmp(buffer, "ADon"))
		return true;
	else
	{
		*error = 'd'; //Exited before sender acknowledged it was done
		return false;
	}
}

//Sends all strings stored within 'messages' through the socket
//-	Will not clear 'messages' when finished. This is so that the data can be recovered
//	or saved should an error occur
//-	Returns true if the message[s] is sent successfully
//-	If an error occurs, the value returned will be false and an error code will be
//	provided through 'error'
bool VirtuVault::sendMessage(char * error)
{
	int numMessages = messages.size();
	
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
	
		} while(strcmp(buffer, "ARdy"));
	
		//Send the next message in the list	
		if(!socket->send(messages[i], error))
			return false;
	}
	
	if(!socket->send("Adon", error)) //Send "Acknowledge Done" code
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
bool VirtuVault::handshake_send(int numMessages, char * error)
{		
	if(!socket->send("QAck", error)) //Send "Query for Acknowledgement" code
		return false;
	
	if(!socket->read(buffer, error))
		return false;
	
	if(!strcmp(buffer, "ARdy")) //Continue with the handshake if the socket has received
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
		*error = 'h'; //Error completing handshake
		return false;
	}
}

//The handshake authentication that must occur on the recieving-end of a message
//before the message is sent
//-	Returns the number of messages that should be expected
//-	If an error occurs, the value returned will be -1 and an error code will be provided
//	through 'error'
int VirtuVault::handshake_receive(char * error)
{
	bool success = false;
	
	if(!socket->receive(buffer, error)) //Read from the socket to buffer
		return -1;
		
	if(!strcmp(buffer, "QAck")) //Continue with the handshake if the socket has received
							    //the "Query for Acknowledgement" code
	{
		//Continue with handshake
		if(!socket->send("ARdy", error)) //Send "Acknowledged and ready" code
			return -1;
			
		if(!socket->receive(buffer, error)) //Wait to receieve precursor info
											//(how many messages to expect)
			return -1;
		
		return atoi(buffer); //Return the number value stored within the string. This
							 //is the precursor info
	}
	else
	{
		*error = 'h'; //Error completing handshake
		return -1;
	}
}

// Client_VirtuVault ********************************************************************

// Server_VirtuVault ********************************************************************

