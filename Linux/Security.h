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
		this->encKey = new_encKey;
		this->set_private_encKey();
	}
	
	~Encryption()
	{}	
	
protected:

	//Private Functions
	void encrypt(char * dest, char * src, const int strlen);
	void decrypt(char * dest, char * src, const int strlen);
	
	//Protected Accessor
	const unsigned long get_public_encKey() const { return this->public_encKey; }
	
	//Protected Mutators
	void set_public_encKey(const unsigned long new_encKey) { this->public_encKey = new_encKey; }
	void set_private_encKey();

private:
	
	//Private Members
	unsigned long public_encKey, private_encKey;
	
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
	
protected:

	//Public Function
	bool check_PasswordInput(const char * input);
	
	//Protected Mutator
	void set_masterPswd(const char * new_masterPswd) { this->masterPswd = new_masterPswd; }

private:

	//Private Member
	char * masterPswd;
};


#endif