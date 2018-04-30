#include "Security.h"


// Encryption **********************************************************************************************************


// Protected -----------------------------------------------------------------------------------------------------------

//Encrypts a given string
//
//	[Output]:	dest -		A reference to the string where the conversion will be stored
//	[Input]:	src -		A reference to the string to be encyrpted
void Encryption::encrypt(char * const dest, const char * const src, const int strlen)
{
	int conversion = rand(), newValue;
	
	//Run through every character in the string
	for(int i = 0; i<strlen; i++)
	{
		if((newValue = (int)src[i] + conversion) > 255) //If the conversion yields >255, count the difference from 0
			newValue -= 255;
		else if((newValue = (int)src[i] + conversion) < 0) //If the conversion yields <0, count the difference from 255
			newValue += 255;
		
		//Assign the finished conversion to the equivalent index in the destination
		*(dest+i) = (char)newValue;
	}
}

//Decrypts a given string
//
//	[Output]:	dest -		A reference to the string where the conversion will be stored
//	[Input]:	src -		A reference to the string to be encyrpted
void Encryption::decrypt(char * const dest, const char * const src, const int strlen)
{
	int conversion = rand(), newValue;
	
	//Run through every character in the string
	for(int i = 0; i<src.length(); i++)
	{
		if((newValue = (int)src[i] - conversion) > 255) //If the conversion yields >255, count the difference from 0
			newValue -= 255;
		else if((newValue = (int)src[i] - conversion) < 0) //If the conversion yields <0, count the difference from 255
			newValue += 255;
		
		//Assign the finished conversion to the equivalent index in the destination
		*(dest+i) = (char)newValue;
	}
}

//Calculates the current session's private key for socket communications based off of what the current session's public
//	key is
void Encryption::set_private_encKey()
{	
	//Calculate the new private encryption key based off of the public encryption key
	//TODO: ^
	private_encKey = 0;
	
	//Seed the random number generator with this new encryption key
	seed();
}

// Private -------------------------------------------------------------------------------------------------------------

//Seeds the random number generator
//
//	[Input]	new_seed -	The new seed value
void Encryption::seed()
{
	srand(private_encKey);
}


// Password ************************************************************************************************************


// Protected -----------------------------------------------------------------------------------------------------------

//Checks to see if the input is equivalent to the stored master password
//
//	[Input]:	input -	A reference to the input string that must be checked against the stored password
//	[Output]:			A boolean value (returns true if the strings match)
bool Password::check_PasswordInput(const char * input)
{
	if(!strcmp(m_masterPswd, input))
		return true;
	else
		return false;
}