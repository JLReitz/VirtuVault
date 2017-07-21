#ifndef CORE_H
#define CORE_H

//Enumerated type for error codes
typedef enum
{
	//Socket related errors
	ERR_SOCK_NOHOST			=	0x01,	//Hostname not found
	ERR_SOCK_OPEN			=	0x02,		//Error opening socket
	ERR_SOCK_CONNECT		=	0x03,	//Error connecting to socket
	ERR_SOCK_WRITE			=	0x04,		//Error writing to socket
	ERR_SOCK_READ			=	0x05,		//Error reading from socket
	
	//Process Related errors
	ERR_VVAULT_HANDSHAKE	=	0x11,	//Error completing communications handshake
	
	MAX_ERR_VAL				=	0xFF	//The maximum value for error codes
} ERR_CODE_T;

//Enumerated type for system codes
typedef enum
{
	FLAG_VVAULT_COMPREEXIT	=	0x01,	//Exited the receive function before ACK_DONE was sent
	
	MAX_SYSCODE_VAL			=	0x0FF	//The maximum value for system codes
}

#endif