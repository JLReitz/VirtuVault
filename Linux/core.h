#ifndef CORE_H
#define CORE_H


// Generic Types -------------------------------------------------------------------------------------------------------

typedef 	unsigned char 	BYTE;
typedef 	unsigned short 	WORD;
typedef		unsigned long	DWORD;

// Enumerated types ----------------------------------------------------------------------------------------------------

//Enumerated type for error codes
typedef enum
{
	ERR_OK					=	0x00
	
	//Socket related errors
	ERR_SOCK_NOHOST			=	0x01,	//Hostname not found
	ERR_SOCK_OPEN			=	0x02,	//Error opening socket
	ERR_SOCK_CONNECT		=	0x03,	//Error connecting to socket
	ERR_SOCK_BIND			=	0x04,
	ERR_SOCK_WRITE			=	0x05,	//Error writing to socket
	ERR_SOCK_READ			=	0x06,	//Error reading from socket
	
	//Message Related Errors
	ERR_MESSAGE_CRC			=	0x10,	//The CRC for the latest message was invalid
	
	ERR_NOOK				=	0xEF,	//Generic "error occured" code
	MAX_ERR_VAL				=	0xFF	//The maximum value for error codes
} ERR_CODE_T;

//Enumerated type for system codes
typedef enum
{	
	SYS_OK					=	0x00,
	
	//Status Reports
	CONNECTION_SUCCESS		=	0x01,
	CONNECTION_FAIL			=	0x02,
	MESSAGE_SUCCESS			=	0x03,
	MESSAGE_FAIL			=	0x04,
	MESSAGE_INPROGRESS		=	0x05,
	HANDSHAKE_SUCCESS		=	0x06,
	HANDSHAKE_FAIL			=	0x07,
	HANDHSAKE_INPROGRESS	=	0x08,
	PROCESS_SUCCESS			=	0x0E,
	PROCESS_FAIL			=	0x0F,
	
	//Flags
	FLAG_VVAULT_COMPREEXIT	=	0xF1,	//Exited the receive function before ACK_DONE was sent
	
	MAX_SYSCODE_VAL			=	0x0FF	//The maximum value for system codes
} SYS_CODE_T;

//Enumerated type for VirtuVault handshake codes
typedef enum
{
	ACK_BEGIN			=	0x00,	//Code to be sent to initiate handshake
	ACK_FOLLOWUP		=	0x01,	//Code to follow an acknowledge request
	ACK_CLOSE			=	0x02,	//Code to close (finish up) the handshake
	
	ACK_READY			=	0x11,
	ACK_RECEIEVED		=	0x12,
	ACK_INVALIDCRC		=	0x13,	//This is thrown when the CRC computed does not equal the CRC sent
	ACK_DONE			=	0x14,
	
	MAX_HANDSHAKE_VAL	=	0x0FF	//The maximum value for handshake codes
} HANDSHAKE_CODE_T;

//Enumerated Type for the handshake state machine
typedef enum
{
	STATE_HANDSHAKE_1	=	0x00,
	STATE_HANDSHAKE_2	=	0x01,
	STATE_HANDSHAKE_3	=	0x02,
	STATE_HANDSHAKE_4	=	0x03,
	STATE_HANDSHAKE_5	=	0x04,
	
	STATE_HANDSHAKE_MAX	=	0x05
} STATE_HANDSHAKE_T;

//Enumerated type for the messaging state machine
typedef enum
{
	
	STATE_MESSAGING_MAX
}STATE_MESSAGING_T;

// Unions --------------------------------------------------------------------------------------------------------------

typedef union 
{
	unsigned char uBYTE;
	signed char sBYTE;
} SIGNED_TO_UNSIGNED_BYTE;

#endif