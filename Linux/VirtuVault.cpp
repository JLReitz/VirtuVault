#include "VirtuVault.h"


// VirtuVault **********************************************************************************************************


// Protected -----------------------------------------------------------------------------------------------------------

//TODO: implement CRC
//Receives all messages from the sender through the socket connection
//
//	[Output]:	error -	An error code (returns ERR_OK if no error occured)
//	[Output]:			A system code (returns MESSAGE_SUCCESS if the process completed successfully)
SYS_CODE_T VirtuVault::receiveMessage(ERR_CODE_T & error)
{
	int numMessages = 0;
	
	//Perform Handshake
	SYS_CODE_T report = handshake_receieve(numeMessages, error); //Handshake
	
	if(report == HANDSHAKE_SUCCESS)
	{
		//Reserve space in 'receieve' for the new messages
		int listSize = receive.size(); //If there's any messages in data already, this will account for that
		receive.reserve(listSize + numMessages);
		
		//Begin receiving messages
		for(int i=0; i<numMessages; i++)
		{
			buffer[0] = ACK_READY;
			if((error = socket->send(&buffer[0], 1))) //Send "ready" code
				return MESSAGE_FAIL;
			
			//Receieve the next message
			if((error = socket->receieve(buffer)))
				return MESSAGE_FAIL;
			
			//Check to see if the sender has sent the "done" code
			if(buffer[0] == ACK_DONE) //If the sender acknowledges it is done sending
				return FLAG_VVAULT_COMPREEXIT; //End the function earlier than expected and return the code that says as such
			
			//Copy the new message into 'receive'
			receive[listSize+i] = buffer;
		}
	}
	else
		return report; //Return HANDSHAKE_FAIL to report where the error occured
	
	//Wait for the "Acknowledge Done" code from the sender
	if((error = socket->receieve(&buffer[0], 1)))
		return MESSAGE_FAIL;
	
	if(buffer[0] == ACK_DONE)
		return MESSAGE_SUCCESS;
	else
		return MESSAGE_FAIL;
}

//TODO: implement CRC
//Sends all strings stored within 'messages' through the socket
//-	Will not clear 'messages' when finished. This is so that the data can be recovered or saved should an error occur
//
//	[Output]:	error -	An error code (returns ERR_OK if no error occured)
//	[Output]:			A system code (returns MESSAGE_SUCCESS if the process completed successfully)
SYS_CODE_T VirtuVault::sendMessage(ERR_CODE_T & error)
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

//The handshake authentication that must occur on the sending-end of a message before the message is sent
//
//	[Input]:	numMessages -	The number of messages to be sent
//	[Output]:	error 		-	An error code (returns ERR_OK if no error occured)
//	[Output]:					A system code (returns HANDSHAKE_SUCCESS if no error occured)
SYS_CODE_T VirtuVault::handshake_send(const int numMessages, ERR_CODE_T & error)
{	
	//TODO: Get rid of
	/*//Send "begin handshake" code
	buffer[0] = ACK_BEGIN
	if((error = socket->send(buffer, 1)))
		return HANDSHAKE_FAIL;
	
	//Wait for "followup" code
	if((error = socket->read(buffer, 1)))
		return HANDSHAKE_FAIL;
	
	if(buffer[0] == ACK_FOLLOWUP)
	{
		//Continue with handshake	
		
		//Send the precursor string
		buffer[0] = (BYTE)numMessages;
		if((error = socket->send(buffer, 1)))
			return HANDSHAKE_FAIL;
		
		//Wait for "receieved" code
		if((error = socket->read(buffer, 1)))
			return HANDSHAKE_FAIL;
		
		if(buffer[0] == ACK_RECEIVED)
		{
			//Send "handshake close" code
			buffer[0] = ACK_CLOSE;
			if((error = socket->send(buffer, 1)))
				return HANDSHAKE_FAIL;
			else
				return HANDSHAKE_SUCCESS;
		}
	}*/
	
	switch(curr_handshake_state)
	{
	case STATE_HANDSHAKE_1:
	
		prev_handshake_state = STATE_HANDSHAKE_1;
		
		//Send "begin handshake" code
		buffer[0] = ACK_BEGIN
		
		if(!(error = socket->send(buffer, 1)))
		{
			curr_handshake_state = STATE_HANDSHAKE_2;
			return HANDSHAKE_INPROGRESS;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_2:
	
		if(prev_handshake_state == STATE_HANDSHAKE_1)
		{
			prev_handshake_state = STATE_HANDSHAKE_2;
			
			//Wait for "followup" code
			if(!(error = socket->read(buffer, 1)) && buffer[0] == ACK_FOLLOWUP)
			{
				curr_handshake_state = STATE_HANDSHAKE_3;
				return HANDSHAKE_INPROGRESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_3:
		
		if(prev_handshake_state == STATE_HANDSHAKE_2)
		{
			prev_handshake_state = STATE_HANDSHAKE_3;
			
			//Send the precursor string
			buffer[0] = (BYTE)numMessages;
			
			if(!(error = socket->send(buffer, 1)))
			{
				curr_handshake_state = STATE_HANDSHAKE_4;
				return HANDSHAKE_INPROGRESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_4:
		
		if(prev_handshake_state == STATE_HANDSHAKE_3)
		{
			prev_handshake_state = STATE_HANDSHAKE_4;
		
			//Wait for "receieved" code
			if((!error = socket->read(buffer, 1)) && buffer[0] == ACK_RECEIVED)
			{
				curr_handshake_state = STATE_HANDSHAKE_5;
				return HANDSHAKE_INPROGRESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_5:
		
		if(prev_handshake_state == STATE_HANDSHAKE_3)
		{
			prev_handshake_state = STATE_HANDSHAKE_4;
			
			//Send "handshake close" code
			buffer[0] = ACK_CLOSE;
			
			if(!(error = socket->send(buffer, 1)))
			{
				curr_handshake_state = STATE_HANDSHAKE_1;
				return HANDSHAKE_SUCCESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case default:
		//TODO: Insert error handling here
		break;
	}
	
	//If the function hasn't returned by now, that means that nothing definitive has happened. Assume the handshake is 
	//	still in progress
	return HANDSHAKE_INPROGRESS;
}

//The handshake authentication that must occur on the recieving-end of a message
//before the message is sent
//
//	[Output]:	numMessages -	The number of messages to expect from the sender
//	[Output]:	error		-	An error code (returns ERR_OK if no error occured)
//	[Output]:				-	A system code (returns HANDSAKE_SUCCESS if no error occured)
SYS_CODE_T VirtuVault::handshake_receive(int & numMessages, RR_CODE_T & error)
{	
	//TODO: Get rid of
	/*//Wait for "begin" code
	if((error = socket->read(buffer, 1)))
		return HANDSHAKE_FAIL;
	
	if(buffer[0] == ACK_BEGIN)
	{
		//Continue with handshake	
		
		//Send "followup" code
		buffer[0] = ACK_FOLLOWUP
		if((error = socket->send(buffer, 1)))
			return HANDSHAKE_FAIL;
		
		//Wait for the number of messages to expect
		if((error = socket->read(buffer, 1)))
			return HANDSHAKE_FAIL;
		
		numMessages = buffer[0];
		
		//Send the "receieved" code
		buffer[0] = ACK_RECEIVED;
		if((error = socket->send(buffer, 1)))
			return HANDSHAKE_FAIL;
		
		//Wait for "handshake close" code
		if((error = socket->read(buffer, 1)))
			return HANDSHAKE_FAIL;
		
		if(buffer[0] == ACK_CLOSE)
			return HANDSHAKE_SUCCESS;
		else
			return HANDSHAKE_FAIL;
	}*/
	
	switch(curr_handshake_state)
	{
	case STATE_HANDSHAKE_1:
	
		prev_handshake_state = STATE_HANDSHAKE_1;
		
		//Wait for "begin" code
		if(!(error = socket->read(buffer, 1)) && buffer[0] == ACK_BEGIN)
		{
			curr_handshake_state = STATE_HANDSHAKE_2;
			return HANDSHAKE_INPROGRESS;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_2:
		
		if(prev_handshake_state == STATE_HANDSHAKE_1)
		{
			prev_handshake_state = STATE_HANDSHAKE_2;
			
			//Send "followup" code
			buffer[0] = ACK_FOLLOWUP
			
			if(!(error = socket->send(buffer, 1)))
			{
				curr_handshake_state = STATE_HANDSHAKE_3;
				return HANDSHAKE_INPROGRESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_3:
		
		if(prev_handshake_state == STATE_HANDSHAKE_2)
		{
			prev_handshake_state = STATE_HANDSHAKE_3;
			
			//Wait for the precursor string
			if(!(error = socket->read(buffer, 1)))
			{
				numMessages = buffer[0];
				
				curr_handshake_state = STATE_HANDSHAKE_4
				return HANDSHAKE_INPROGRESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_4:
		
		if(prev_handshake_state == STATE_HANDSHAKE_3)
		{
			prev_handshake_state = STATE_HANDSHAKE_4;
			
			//Send the "receieved" code
			buffer[0] = ACK_RECEIVED;
			
			if(!(error = socket->send(buffer, 1)))
			{
				curr_handshake_state = STATE_HANDSHAKE_5;
				return HANDSHAKE_INPROGRESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case STATE_HANDSHAKE_5:
		
		if(prev_handshake_state == STATE_HANDSHAKE_4)
		{
			prev_handshake_state = STATE_HANDSHAKE_5;
			
			//Wait for "handshake close" code
			if(!(error = socket->read(buffer, 1)) && buffer[0] == ACK_CLOSE)
			{
				curr_handshake_state = STATE_HANDSHAKE_1;
				return HANDSAKE_SUCCESS;
			}
			else
				curr_handshake_state = STATE_HANDSHAKE_MAX;
		}
		else
			curr_handshake_state = STATE_HANDSHAKE_MAX;
		
		break;
		
	case default:
		//TODO: Insert error handling here
		break;
	}
	
	//If the function hasn't returned by now, that means that nothing definitive has happened. Assume the handshake is 
	//	still in progress
	return HANDSHAKE_INPROGRESS;
}


// Client_VirtuVault ***************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

SYS_CODE_T Client_VirtuVault::connect(ERR_CODE_T & error)
{
}

SYS_CODE_T & error Client_VirtuVault::run_Process(const char * topic, const char * options, ERR_CODE_T & error)
{
	return false;
}

// Protected ----------------------------------------------------------------------------

void Client_VirtuVault::load_returned()
{
}

SYS_CODE_T Client_VirtuVault::connectSocket(ERR_CODE_T & error)
{
	return false;
}

SYS_CODE_T Client_VirtuVault::connectSocket(ERR_CODE_T & error)
{
	return false;
}

SYS_CODE_T Client_VirtuVault::setup(ERR_CODE_T & error)
{
	return false;
}

SYS_CODE_T Client_VirtuVault::startup(ERR_CODE_T & error)
{
	return false;
}


// Server_VirtuVault ***************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

SYS_CODE_T Server_VirtuVault::connect(ERR_CODE_T & error)
{
}

SYS_CODE_T Server_VirtuVault::run_Process(ERR_CODE_T & error)
{
	return false;
}

// Protected -----------------------------------------------------------------------------------------------------------

bool Server_VirtuVault::setup(ERR_CODE_T & error)
{
	return false;
}

SYS_CODE_T Server_VirtuVault::startup(ERR_CODE_T & error)
{
	return false;
}
