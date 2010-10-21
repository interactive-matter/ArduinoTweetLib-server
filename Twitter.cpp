/*
  Twitter.cpp - Arduino library to Post messages to Twitter using OAuth.
  Copyright (c) NeoCat 2010. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#include "Twitter.h"
#include <string.h>
#include <stdlib.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>

char* twitter_api_server = "api.supertweet.net";
char* twitter_search = "search.twitter.com";

Twitter::Twitter(char *name, char *password) : name(name), password(password)
{
}

bool Twitter::post(char *msg)
{
  HTTPClient client = HTTPClient(twitter_api_server,name,password);
  FILE* stream = client.postURI("/update",msg);
  statusCode = client.getLastReturnCode();
  client.closeStream(stream);
  return (statusCode==200);
}

char Twitter::search(char* searchTerm) {
  //different search - hence different lastId
  if (strcmp(lastSearch,searchTerm)!=0) {
      lastSearch=searchTerm;
      if (lastId!=NULL) {
          free(lastId);
          lastId=NULL;
      }
  }

  char requestbuffer[64];
  if (lastId==NULL) {
      sprintf(requestbuffer,"/search.json\?result_type=recent&q=%s",searchTerm);
  } else {
      sprintf(requestbuffer,"/search.json\?result_type=recent&q=%s&since_id=%s",searchTerm,lastId);
  }

  HTTPClient client = HTTPClient(twitter_search);
	Serial.println(requestbuffer);
	client.debug(-1);
  FILE* stream = client.getURI(requestbuffer);
	Serial.print("answer ");
	Serial.print(client.getLastReturnCode());
  char id_buffer[16];
  char results=0;
  //while we are able to scan the id
  while (fscanf(stream,"\"id\":%s",id_buffer)==1) {
      results++;
      if (lastId!=NULL) {
          //if the lasId is longer than this id everything is cool
          if (strlen(lastId)>strlen(id_buffer)) {
              //cool nothing to do
              //if the new id is longer we use the longer id
          } else if (strlen(lastId)<strlen(id_buffer)) {
              free(lastId);
              lastId=NULL;
              //ok both have the same length - now we can check it lexigraphically
          } else if (strcmp(lastId,id_buffer)<0) {
              free(lastId);
              lastId=NULL;
          }
      }
      //we have either decided that this id is bigger or we never had an ID
      if (lastId==NULL) {
          lastId = (char*) malloc(strlen(id_buffer)*sizeof(char));
          strcpy(lastId,id_buffer);
      }
  }
  return results;
}
