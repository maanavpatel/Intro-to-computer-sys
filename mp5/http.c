#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "http.h"


/**
 * httprequest_parse_headers
 * 
 * Populate a `req` with the contents of `buffer`, returning the number of bytes used from `buf`.
 */



ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) {
  char *theRest = malloc(sizeof(char*));
  char *buff = malloc(buffer_len);
  memcpy(buff,buffer,buffer_len);
  req->head = NULL;
  req->tail = NULL;
  req->payload = NULL; 
  printf("BUFFER: \n%s\n=======\n", buff);

  char * token;
  token = strtok_r(buff, " ", theRest);
  


  req->action = strdup(token);
  printf("action: %s\n",req->action);

  token = strtok_r(NULL, " ",theRest);

  req->path = strdup(token);
  printf("path: '%s'\n",req->path);

  token = strtok_r(NULL, "\r\n", theRest);


  req->version = strdup(token);
  printf("version: '%s'\n",req->version);
  
  // token = strtok_r(NULL, "\r\n", saveptr1);
  // printf("token = '%s'\n", token);
  // loop through the string to extract all other token

  size_t i = 0;
  while(token != NULL) {
      token = strtok_r(NULL, "\r\n",theRest);
      // printf("i = %ld\n", i);
      if (token != NULL) {  //map header 'key: value' pairs
        // printf("token = '%s'\n", token);

        char* line = strdup(token);
        if (strstr(line,":")) {
          char* key = strtok(line,":");

          char* val = strtok(NULL," ");
          printf("\tkey: '%s'\t\tval: '%s'\n",key,val);

          if (key != NULL && val != NULL) {
            if (req->head == NULL) { //first node
              req->head = malloc(sizeof(HeaderMap));
              req->head->key = malloc(strlen(key)+1);
              req->head->value = malloc(strlen(val)+1);
              strcpy(req->head->key, key);
              strcpy(req->head->value, val);

              req->tail = req->head;
              req->head->next = req->tail;
              req->tail->next = NULL;
              // printf("{*} FIRST: '%s':'%s'\n",req->head->key,req->head->value);
            } else { //attach at tail
              HeaderMap * h =  malloc(sizeof(HeaderMap));
              req->tail->next = h;
              req->tail = h;

              req->tail->key = malloc(strlen(key)+1);
              req->tail->value = malloc(strlen(val)+1);
              strcpy(req->tail->key, key);
              strcpy(req->tail->value, val);
              // printf("{*} attached at tail: '%s':'%s'\n",req->tail->key,req->tail->value);
            }

            

             if (strcmp(key, "Content-Length") == 0) { // the rest is just content;

              char * pay = strtok_r(NULL, "\r\n\r\n", theRest);
              req->payload = strdup(pay);
              printf("PAYLOAD: \n**%s**\n-_-_-_-\n", req->payload);

              break;
            }
          } else {
              // char * pay = strtok_r(NULL, "", theRest);           
              break;
          }
        }
        
      }
      i++;
   }
  printf("****done****\n");

  if (req != NULL) {
    return sizeof(req->version) + sizeof(req->path) + sizeof(req->action);
  }

  return -1;
}


/**
 * httprequest_read
 * 
 * Populate a `req` from the socket `sockfd`, returning the number of bytes read to populate `req`.
 */
ssize_t httprequest_read(HTTPRequest *req, int sockfd) {
    // size_t fsize = lseek(sockfd, 0, SEEK_END);
    printf("SOCKET %d\n",sockfd);

    char* buff = malloc(4096);
    size_t bytesUsed = 0;

    while(1) {      
      ssize_t len = read(sockfd, buff, 4096); //read file
      bytesUsed += len; //increase bytes read by len

      // printf("LEN %ld,\t BYTESUSED: %ld",len, bytesUsed);
      if (len == 0) { //nothing more read
        break;
      }
      
      if (bytesUsed % 4096  < 100) { //if the buffer is almost full
        printf("\nrealloc: bytes m/ 4096: %d\n", bytesUsed%4096);

        buff = realloc(buff, sizeof(buff) + 4096);
      }

      printf("\n== read() %ld bytes ==\n", bytesUsed);
      // printf("\n~~~~buffer~~~~~\n%s~~~~~\n", buff);
    }


    printf("\n+++ req, buff*size, bytesUsed: %p, %d, %d +++\n", req, strlen(buff), bytesUsed);

    
  return httprequest_parse_headers(req, buff, bytesUsed);
}


/**
 * httprequest_get_action
 * 
 * Returns the HTTP action verb for a given `req`.
 */
const char *httprequest_get_action(HTTPRequest *req) {
  return req->action;
}


/**
 * httprequest_get_action
 * 
 * Returns the value of the HTTP header `key` for a given `req`.
 */
const char *httprequest_get_header(HTTPRequest *req, const char *key) {
  printf(">>> KEY: '%s'\n",key);

  HeaderMap* curr = req->head;
  while (curr != NULL) {
    if (strcmp(curr->key, key) == 0) {
      printf(">>> '%s' : '%s'\n",key, curr->value);
      return curr->value;
    }
    curr = curr->next;
  }
  printf(">>> KEY NOT FOUND");
  return NULL;
}


/**
 * httprequest_get_action
 * 
 * Returns the requested path for a given `req`.
 */
const char *httprequest_get_path(HTTPRequest *req) {
  return req->path;
}


/**
 * httprequest_get_action
 * 
 * Destroys a `req`, freeing all associated memory.
 */
void httprequest_destroy(HTTPRequest *req) {

}
