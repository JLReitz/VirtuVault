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
	ERR_MESSAGE_CRC			=	0x0A,	//The CRC for the latest message was invalid
	
	ERR_NOOK				=	0x0E,	//Generic "error occured" code
	
	MAX_ERR_VAL				=	0x0F	//The maximum value for error codes
} ERR_CODE_T;

//Enumerated type for system codes
typedef enum
{	
	SYS_OK					=	0x10,
	
	CONNECTION_SUCCESS		=	0x11,
	CONNECTION_FAIL			=	0x12,
	MESSAGE_SUCCESS			=	0x13,
	MESSAGE_FAIL			=	0x14,
	MESSAGE_INPROGRESS		=	0x15,
	HANDSHAKE_SUCCESS		=	0x16,
	HANDSHAKE_FAIL			=	0x17,
	HANDHSAKE_INPROGRESS	=	0x18,
	
	PROCESS_SUCCESS			=	0x1D,
	PROCESS_FAIL			=	0x1E,
	
	MAX_SYSCODE_VAL			=	0x1F	//The maximum value for system codes
} SYS_CODE_T;

//Enumerated type for VirtuVault handshake codes
typedef enum
{
	ACK_BEGIN			=	0x80,
	ACK_FOLLOWUP		=	0x81,
	ACK_DONE			=	0x82,
	
	MAX_HANDSHAKE_VAL	=	0x85	//The maximum value for handshake codes
} HANDSHAKE_CODE_T;

//Enumerated type for the messaging state machine
typedef enum
{
	STATE_MESSAGE_1		=	0xA0,
	STATE_MESSAGE_2		=	0xA1,
	STATE_MESSAGE_3		=	0xA2,
	STATE_MESSAGE_4		=	0xA3,
	STATE_MESSAGE_5		=	0xA4,
	STATE_MESSAGE_6		=	0xA5,
	STATE_MESSAGE_7		=	0xA6,
	STATE_MESSAGE_8		=	0xA7,
	STATE_MESSAGE_9		=	0xA8,
	STATE_MESSAGE_10	=	0xA9,
	
	STATE_MESSAGE_MAX	=	0xAA
} STATE_MESSAGE_T;

//Enumerated Type for the handshake state machine
typedef enum
{
	STATE_HANDSHAKE_1	=	0x90,
	STATE_HANDSHAKE_2	=	0x91,
	STATE_HANDSHAKE_3	=	0x92,
	
	STATE_HANDSHAKE_MAX	=	0x95,	
} STATE_HANDSHAKE_T;

// Unions --------------------------------------------------------------------------------------------------------------

typedef union 
{
	unsigned char uBYTE;
	signed char sBYTE;
} SIGNED_TO_UNSIGNED_BYTE;

#endif