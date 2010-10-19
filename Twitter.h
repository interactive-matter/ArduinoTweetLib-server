/*
  Twitter.cpp - Arduino library to Post messages to Twitter using OAuth.
  Copyright (c) NeoCat 2010. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef TWITTER_H
#define TWITTER_H

#include <inttypes.h>
#include <avr/pgmspace.h>

class Twitter
{
private:
	uint8_t parseStatus;
	int statusCode;
	char *name;
        char *password;
	char* lastSearch;
	char* lastId;

public:
	Twitter(char *name, char*password);
	
	bool post(char *msg);
	int  status(void) { return statusCode; }
	char search(char* term);
};

#endif	//TWITTER_H
