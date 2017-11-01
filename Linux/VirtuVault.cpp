#include "VirtuVault.h"


// VirtuVault **********************************************************************************************************


// Protected -----------------------------------------------------------------------------------------------------------

//TODO: implement CRC
//Receives all messages from the sender through the socket connection
//
//	[Output]:	error -	An error code (returns ERR_OK if no error occured)
//	[Output]:			A system code (returns MESSAGE_SUCCESS if the process completed successfully)
CODE_SYS_T VirtuVault::receiveMessage(CODE_ERROR_T & error)
{
	static unsigned int numMessages = 0;
	
	//TODO: Add state machine code
	switch(curr_message_state)
	{
		case STATE_MESSAGE_1: //Handshake
			
			STATE_HANDSHAKE_T handshake_status = handshake_receive(error);
			
			prev_message_state = STATE_HANDSHAKE_1;
			
			if(handshake_status == HANDSHAKE_INPROGRESS)
			{
				curr_message_state = STATE_MESSAGE_1;
				return MESSAGE_INPROGRESS;
			}
			else if (handshake_status == HANDSHAKE_SUCCESS)
			{
				curr_message_state = STATE_MESSAGE_2;
				return MESSAGE_INPROGRESS;
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_2: //Rx Number of Messages
			
			if(prev_message_state == STATE_MESSAGE_1)
			{
				prev_message_state = STATE_MESSAGE_2;
				
				if(!(error = socket->read(buffer, 1)) && ((numMessages = buffer[0]) > 0))
					return MESSAGE_INPROGRESS;
				else
					curr_handshake_state = STATE_HANDSHAKE_MAX;
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_3: //Send Acknowledgement
		
			if(prev_message_state == STATE_MESSAGE_2)
			{
				prev_message_state = STATE_MESSAGE_3;
				
				buffer[0] = (BYTE)MESSAGE_ACK_3;
				
				if(!(error = socket->send(buffer, 1)))
				{
					curr_message_state = STATE_MESSAGE_4;
					return MESSAGE_INPROGRESS;
				}
				else
					curr_message_state = STATE_MESSAGE_MAX;
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_4:
			
			if(prev_message_state == STATE_MESSAGE_3)
			{
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_5:
			
			if(prev_message_state == STATE_MESSAGE_4)
			{
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_6:
			
			if(prev_message_state == STATE_MESSAGE_5)
			{
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_7:
			
			if(prev_message_state == STATE_MESSAGE_6)
			{
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_8:
			
			if(prev_message_state == STATE_MESSAGE_7)
			{
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_9:
			
			if(prev_message_state == STATE_MESSAGE_8)
			{
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_10:
			
			if(prev_message_state == STATE_MESSAGE_9)
			{
				
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
		
		default:
			//TODO: Insert error handling here; state2, state3, 
			break;
	}
	
	return MESSAGE_INPROGRESS;
}

//TODO: implement CRC
//Sends all strings stored within 'messages' through the socket
//-	Will not clear 'messages' when finished. This is so that the data can be recovered or saved should an error occur
//
//	[Output]:	error -	An error code (returns ERR_OK if no error occured)
//	[Output]:			A system code (returns MESSAGE_SUCCESS if the process completed successfully)
CODE_SYS_T VirtuVault::sendMessage(CODE_ERROR_T & error)
{
	//TODO: Add state machine code
	switch(curr_message_state)
	{
		case STATE_MESSAGE_1: //Handshake
			
			STATE_HANDSHAKE_T handshake_status = handshake_send(error);
			
			prev_message_state = STATE_HANDSHAKE_1;
			
			if(handshake_status == HANDSHAKE_INPROGRESS)
			{
				curr_message_state = STATE_MESSAGE_1;
				return MESSAGE_INPROGRESS;
			}
			else if (handshake_status == HANDSHAKE_SUCCESS)
			{
				curr_message_state = STATE_MESSAGE_2;
				return MESSAGE_INPROGRESS;
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_2: //TX Number of Messages
			
			if(prev_message_state == STATE_MESSAGE_1)
			{
				prev_message_state = STATE_MESSAGE_2;
				
				buffer[0] = (BYTE)results.size();
				
				if(!(error = socket->send(buffer, 1)))
				{
					curr_message_state = STATE_MESSAGE_4;
					return MESSAGE_INPROGRESS;
				}
				else
					curr_message_state = STATE_MESSAGE_MAX;
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_3: //Receive Acknowledgement
			
			if(prev_message_state == STATE_MESSAGE_2)
			{
				prev_message_state = STATE_MESSAGE_3;
				
				if(!(error = socket->read(buffer, 1)) && (buffer[0]) == MESSAGE_ACK_3))
					return MESSAGE_INPROGRESS;
				else
					curr_handshake_state = STATE_HANDSHAKE_MAX;
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return MESSAGE_FAIL;
			}
			
			break;
			
		case STATE_MESSAGE_4:
			
			break;
			
		case STATE_MESSAGE_5:
			
			break;
			
		case STATE_MESSAGE_6:
			
			break;
			
		case STATE_MESSAGE_7:
			
			break;
			
		case STATE_MESSAGE_8:
			
			break;
			
		case STATE_MESSAGE_9:
			
			break;
			
		case STATE_MESSAGE_10:
			
			break;
	}
	
	return MESSAGE_INPROGRESS;
}

//The handshake authentication that must occur on the sending-end of a message before the message is sent
//
//	[Input]:	numMessages -	The number of messages to be sent
//	[Output]:	error 		-	An error code (returns ERR_OK if no error occured)
//	[Output]:					A system code (returns HANDSHAKE_SUCCESS if no error occured)
CODE_SYS_T VirtuVault::handshake_send(CODE_ERROR_T & error)
{	
	switch(curr_handshake_state)
	{
	case STATE_HANDSHAKE_1:
	
		prev_handshake_state = STATE_HANDSHAKE_1;
		
		//Send "begin handshake" code
		buffer[0] = HANDSHAKE_ACK_BEGIN;
		
		if(!(error = socket->send(buffer, 1)))
		{
			curr_handshake_state = STATE_HANDSHAKE_2;
			return HANDSHAKE_INPROGRESS;
		}
		else
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			return HANDSHAKE_FAIL;
		}
		
		break;
		
	case STATE_HANDSHAKE_2:
	
		if(prev_handshake_state == STATE_HANDSHAKE_1)
		{
			prev_handshake_state = STATE_HANDSHAKE_2;
			
			//Wait for "followup" code
			if(!(error = socket->read(buffer, 1)) && buffer[0] == HANDSHAKE_ACK_FOLLOWUP)
			{
				curr_handshake_state = STATE_HANDSHAKE_3;
				return HANDSHAKE_INPROGRESS;
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return HANDSHAKE_FAIL;
			}
		}
		else
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			return HANDSHAKE_FAIL;
		}
		
		break;
		
	case STATE_HANDSHAKE_3:
		
		if(prev_handshake_state == STATE_HANDSHAKE_2)
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			prev_handshake_state = STATE_HANDSHAKE_3;
			
			//Send the precursor string
			buffer[0] = HANDSHAKE_ACK_DONE;
			
			if(!(error = socket->send(buffer, 1)))
				return HANDSHAKE_SUCCESS;
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return HANDSHAKE_FAIL;
			}
		}
		else
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			return HANDSHAKE_FAIL;
		}
		
		break;
		
	default:
		//If the handshake fails at any point, default state is entered.
		return HANDSHAKE_FAIL; //No room for error in the handshake. No multiple attempts
		
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
CODE_SYS_T VirtuVault::handshake_receive(CODE_ERROR_T & error)
{
	switch(curr_handshake_state)
	{
	case STATE_HANDSHAKE_1:
	
		prev_handshake_state = STATE_HANDSHAKE_1;
		
		//Wait for "begin" code
		if(!(error = socket->read(buffer, 1)) && buffer[0] == HANDSHAKE_ACK_BEGIN)
		{
			curr_handshake_state = STATE_HANDSHAKE_2;
			return HANDSHAKE_INPROGRESS;
		}
		else
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			return HANDSHAKE_FAIL;
		}
		
		break;
		
	case STATE_HANDSHAKE_2:
		
		if(prev_handshake_state == STATE_HANDSHAKE_1)
		{
			prev_handshake_state = STATE_HANDSHAKE_2;
			
			//Send "followup" code
			buffer[0] = HANDSHAKE_ACK_FOLLOWUP;
			
			if(!(error = socket->send(buffer, 1)))
			{
				curr_handshake_state = STATE_HANDSHAKE_3;
				return HANDSHAKE_INPROGRESS;
			}
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return HANDSHAKE_FAIL;
			}
		}
		else
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			return HANDSHAKE_FAIL;
		}
		
		break;
		
	case STATE_HANDSHAKE_3:
		
		if(prev_handshake_state == STATE_HANDSHAKE_2)
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			prev_handshake_state = STATE_HANDSHAKE_3;
			
			//Wait for "handshake close" code
			if(!(error = socket->read(buffer, 1)) && buffer[0] == HANDSHAKE_ACK_DONE)
				return HANDSAKE_SUCCESS;
			else
			{
				curr_handshake_state = STATE_HANDSHAKE_MAX;
				return HANDSHAKE_FAIL;
			}
		}
		else
		{
			curr_handshake_state = STATE_HANDSHAKE_MAX;
			return HANDSHAKE_FAIL;
		}
		
		break;
		
	case default:
		//If the handshake fails at any point, default state is entered.
		return HANDSHAKE_FAIL; //No room for error in the handshake. No multiple attempts
		
		break;
	}
	
	//If the function hasn't returned by now, that means that nothing definitive has happened. Assume the handshake is 
	//	still in progress
	return HANDSHAKE_INPROGRESS;
}


// Client_VirtuVault ***************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

CODE_SYS_T Client_VirtuVault::connect(CODE_ERROR_T & error)
{
}

CODE_SYS_T & error Client_VirtuVault::run_Process(const char * topic, const char * options, CODE_ERROR_T & error)
{
	return false;
}

// Protected ----------------------------------------------------------------------------

void Client_VirtuVault::load_returned()
{
}

CODE_SYS_T Client_VirtuVault::connectSocket(CODE_ERROR_T & error)
{
	return false;
}

CODE_SYS_T Client_VirtuVault::connectSocket(CODE_ERROR_T & error)
{
	return false;
}

CODE_SYS_T Client_VirtuVault::setup(CODE_ERROR_T & error)
{
	return false;
}

CODE_SYS_T Client_VirtuVault::startup(CODE_ERROR_T & error)
{
	return false;
}


// Server_VirtuVault ***************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

CODE_SYS_T Server_VirtuVault::connect(CODE_ERROR_T & error)
{
}

CODE_SYS_T Server_VirtuVault::run_Process(CODE_ERROR_T & error)
{
	return false;
}

// Protected -----------------------------------------------------------------------------------------------------------

bool Server_VirtuVault::setup(CODE_ERROR_T & error)
{
	return false;
}

CODE_SYS_T Server_VirtuVault::startup(CODE_ERROR_T & error)
{
	return false;
}
