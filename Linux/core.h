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
	
	MAX_ERR_VAL				=	0x0F
} CODE_ERROR_T;

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
	
	CODE_SYS_MAX			=	0x1F
} CODE_SYS_T;

typedef enum
{
	MESSAGE_ACK_3	=	0x83,
	MESSAGE_ACK_5	=	0x85,
	
	CODE_MESSAGE_T	=	0x8F
} MESSAGE_CODE_T;

//Enumerated type for VirtuVault handshake codes
typedef enum
{
	HANDSHAKE_ACK_BEGIN		=	0x90,
	HANDSHAKE_ACK_FOLLOWUP	=	0x91,
	HANDSHAKE_ACK_DONE		=	0x92,
	
	CODE_HANDSHAKE_T		=	0x95
} CODE_HANDSHAKE_T;

typedef enum
{
	STATE_SETUP_1		=	0xA0,
	STATE_SETUP_2		=	0xA1,
	STATE_SETUP_3		=	0xA2,
	STATE_SETUP_4		=	0xA3,
	STATE_SETUP_5		=	0xA4,
	
	STATE_SETUP_MAX		=	0xA5
} STATE_SETUP_T;

typedef enum
{
	STATE_PROCESS_1		=	0xB0,
	STATE_PROCESS_2		=	0xB1,
	STATE_PROCESS_3		=	0xB2,
	STATE_PROCESS_4		=	0xB3,
	STATE_PROCESS_5		=	0xB4,
	STATE_PROCESS_6		=	0xB5,
	STATE_PROCESS_7		=	0xB6,
	
	STATE_PROCESS_MAX 	=	0xBA 
} STATE_PROCESS_T;

//Enumerated type for the messaging state machine
typedef enum
{
	STATE_MESSAGE_1		=	0xC0,
	STATE_MESSAGE_2		=	0xC1,
	STATE_MESSAGE_3		=	0xC2,
	STATE_MESSAGE_4		=	0xC3,
	STATE_MESSAGE_5		=	0xC4,
	STATE_MESSAGE_6		=	0xC5,
	STATE_MESSAGE_7		=	0xC6,
	STATE_MESSAGE_8		=	0xC7,
	STATE_MESSAGE_9		=	0xC8,
	STATE_MESSAGE_10	=	0xC9,
	
	STATE_MESSAGE_MAX	=	0xCA
} STATE_MESSAGE_T;

//Enumerated Type for the handshake state machine
typedef enum
{
	STATE_HANDSHAKE_1	=	0xD0,
	STATE_HANDSHAKE_2	=	0xD1,
	STATE_HANDSHAKE_3	=	0xD2,
	
	STATE_HANDSHAKE_MAX	=	0xD5
} STATE_HANDSHAKE_T;

// Unions --------------------------------------------------------------------------------------------------------------

typedef union 
{
	unsigned char uBYTE;
	signed char sBYTE;
} SIGNED_TO_UNSIGNED_BYTE;

#endif