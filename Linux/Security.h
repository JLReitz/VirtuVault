#ifndef SECURITY_H
#define SECURITY_H


#include <math.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "core.h"

using namespace std;


// Encryption Class ****************************************************************************************************

class Encryption
{
public:

	Encryption(const unsigned long new_encKey = 0)
	{
		this->m_public_encKey = new_encKey;
		this->set_private_encKey();
	}
	
	~Encryption()
	{}	
	
protected:

	//Private Functions
	void encrypt(char * const dest, const char * const src, const int strlen);
	void decrypt(char * const dest, const char * const src, const int strlen);
	
	//Protected Accessor
	const unsigned long get_public_encKey() const { return this->m_public_encKey; }
	
	//Protected Mutators
	void set_public_encKey(const DWORD new_encKey) { this->m_public_encKey = new_encKey; }

private:
	
	//Private Members
	unsigned long m_public_encKey, m_private_encKey;
	
	//Private Mutator
	void seed();
	
};

// Password Manager Class **********************************************************************************************

class Password
{
public:

	Password(const char * new_masterPswd = "")
	{
		this->set_masterPswd(new_masterPswd);
	}
	
	Password()
	{}
	
	//Public Function
	bool check_PasswordInput(const char * input);
	
protected:
	
	//Protected Mutator
	void set_masterPswd(const char * new_masterPswd) { this->masterPswd = new_masterPswd; }

private:

	//Private Member
	char * m_masterPswd;
};


#endif