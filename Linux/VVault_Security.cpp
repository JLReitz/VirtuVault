#include "VVault_Security.h"


// VVault_Encryption ***************************************************************************************************


// Public --------------------------------------------------------------------------------------------------------------

//Encrypts a given string from the VVault application
//
//	[Output]:	dest -		A reference to the string where the conversion will be stored
//	[Input]:	src -		A reference to the string to be encyrpted
void VVault_Encryption::VVault_encrypt(string & dest, const string & src)
{
	char dest_buffer[255];
	
	encrypt(dest_buffer, src.c_str(), src.length());
	
	dest = dest_buffer;
}

//Encrypts a given string from the VVault application
//
//	[Output]:	dest -		A reference to the string where the conversion will be stored
//	[Input]:	src -		A reference to the string to be encyrpted
void VVault_Encryption::VVault_decrypt(string & dest, const string & src)
{
	char dest_buffer[255];
	
	decrypt(dest_buffer, src.c_str(), src.length());
	
	dest = dest_buffer;
}

//Cyclic Redundancy Check for testing the integrity of messages being sent over the socket connection
//-	This is not a true CRC, but rather a modified version that works (I think) better for the application
//
//	[Input/Output]: crc - 	The CRC value that results from the input message
//	[Input]:		str	-	A reference to the string that the CRC must be done on
void VVault_Encryption::crc_set(BYTE & crc, const string & str)
{
	//Generate byte-sized variables to be used in CRC calculations
	BYTE crc_compute = 0, crc_compare = 0b10010001;
	
	//Iterate through the length of str and perform an 'xor' operation on each character
	for(size_t i=0; i<str.length(); i++)
	{
		//'And' the current character with crc_compare
		BYTE crc_compared = str[i] & crc_compare;
		
		//Update crc_compute by 'xor'ing it with the result
		crc_compute ^= crc_compared;
	}
	
	//Assign crc_compute to the output crc
	crc = crc_compute;
}

//Checks against a provided CRC for a specific message provided by the socket connection
//-	This is to be used to check the validity of the received messages
//
//	[Input]:	crc -	The provided CRC value to be checked against
//	[Input]:	str -	A reference to the string that the CRC must be done on
CODE_ERROR_T VVault_Encryption::crc_check(const BYTE & crc, const string & str)
{
	//Generate byte-sized variables to be used in CRC calculations
	BYTE crc_compute = 0, crc_compare = 0b10010001;
	
	//Find the length of the string
	size_t strlen = strlen(str);
	
	//Iterate through the length of str and perform an 'xor' operation on each character
	for(size_t i=0; i<strlen; i++)
	{
		//'And' the current character with crc_compare
		BYTE crc_compared = str[i] & crc_compare;
		
		//Update crc_compute by 'xor'ing it with the result
		crc_compute ^= crc_compared;
	}
	
	//Compare the computed CRC to the one provided
	if(crc == crc_compared)
		return ERR_OK; //Return okay message if they are identical
	else
		return ERR_MESSAGE_CRC; //Return the code for an invalid CRC if not
}


// VVault_Password *****************************************************************************************************

//Checks to see if the input is equivalent to the stored master password
//
//	[Input]:	input -	A reference to the input string that must be checked against the stored password
//	[Output]:			A boolean value (returns true if the strings match)
bool VVault_Password::VVault_Check_PasswordInput(const string & input)
{
	return check_PasswordInput(input.c_str());
}