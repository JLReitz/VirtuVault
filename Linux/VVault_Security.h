#ifndef VVault_Security_H
#define VVault_Security_H


#include "Security.h"


// Encryption Class ****************************************************************************************************

class VVault_Encryption : public Encryption
{
public:

	VVault_Encryption(const unsigned long new_encKey = 0) : Encryption(new_encKey)
	{}
	~VVault_Encryption()
	{}

	//Public Functions
	void VVault_encrypt(string & dest, string & src);
	void VVault_decrypt(string & dest, string & src);
	void crc_set(BYTE & crc, const string & str);
	CODE_ERROR_T crc_check(const BYTE & crc, const string & str);
};

// Password Class ******************************************************************************************************

class VVault_Password : public Password
{
public:

	VVault_Password(const string & new_masterPswd) : Password(new_masterPswd)
	{}
	~VVault_Password()
	{}
	
	//Public Function
	bool VVault_Check_PasswordInput(const string & input);
};


#endif