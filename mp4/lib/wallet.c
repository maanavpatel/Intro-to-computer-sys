#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wallet.h"


/**
 * NOTES:
 * 
 * part 1 make wallet
 * 
 * degree to get a tophat
 * 
 *  degree.c has necessary rules
 * 
 */

/**
 * Initializes an empty wallet.
 */


void wallet_init(wallet_t *wallet) {
    // pthread_mutex_init(&wallet->lock,NULL);  //initialize the mutex (unblocked)
    // wallet->head = NULL;  //set head to NULL (head is first resource)
    wallet->head = NULL;
    pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;
    wallet->lock = l;
}



/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  wallet_resource *curr = wallet->head;
  while(curr != NULL) {
    if (strcmp(curr->resource_name,resource) == 0 ){  //find the resource
      return curr->amount;
    }
    curr = curr->next;
  } 
  return 0;
}

/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource can be satisfied.
 *   (Ths function MUST WAIT until the wallet has enough resources to satisfy the request;
 *    there are several ways to accomplish this waiting and it does not have to be fancy.)
 */
void wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  // printf("wallet: %p, resource: %s, delta: %d \n\t", wallet, resource, delta);
  if (wallet->head == NULL) {   //no resources in wallet
    // printf("no resources\t");
    wallet_resource *r = malloc(sizeof(wallet_resource));
    r->resource_name = resource;
    r->amount = 0;
    wallet->head = r;
    if (delta >= 0) {
      r->amount += delta;
      return;
    }
  }

  /**
   * if  head dne :
   *    create resource and set head
   *    if delta non neg:
   *        
   * other resources exist:
   * 
   *    find the resource
   * 
   *    can't find resource:
   *      - create resource and add to the head
   *    
   *    LOOP
   *      lock thread
   *        if adding delta will stay positive
   *           END LOOP
   *      Unlock thread   (allows for other threads to change)
   *    END LOOP
   * 
   *      
   * 
   */      

  //resources exist

  //find resource
  wallet_resource* curr = wallet->head;
  while(curr != NULL) { 
    if (strcmp(curr->resource_name,resource) == 0 ){  //find the resource
      break;   
    }
    curr = curr->next;
  } 

  //resource dne
  if (curr == NULL) {
    wallet_resource* r = malloc(sizeof(wallet_resource));
    r->resource_name = resource;
    r->amount = 0;
    r->next = wallet->head;
    wallet->head = r;
    curr = r;
  }

  int waiting_for_change = 1;   //true
  while (waiting_for_change) {
    pthread_mutex_lock(&wallet->lock);
    if (curr->amount + delta >= 0) {
      curr->amount += delta;
      waiting_for_change = 0;
    }
    pthread_mutex_unlock(&wallet->lock);
  }
  return;
  

  //wallet contains some resources already

  // if (delta > 0) {  
  //   // printf("\nresources exist and delta positive\n");
  //         //delta positive only adding
  //   wallet_resource* curr = wallet->head;

  //     while(curr != NULL) {
  //       if (strcmp(curr->resource_name,resource) == 0 ){  //find the resource
  //         break;
  //       }
  //       curr = curr->next;
  //     } 

  //     //mutex locks and stufff
  //     pthread_mutex_lock(&wallet->lock);
  //     if (curr == NULL) {   //resource not found, insert into wallet
  //       wallet_resource* r = malloc(sizeof(wallet_resource));
  //       r->resource_name = resource;
  //       r->amount = delta;

  //       //insert at head
  //       r->next = wallet->head;
  //       wallet->head = r;
  //     } else {
  //       //increase resource amount
  //       curr->amount += delta;
  //     }      
  //     pthread_mutex_unlock(&wallet->lock);

  // } else 
    // printf("\nresources exist and delta negative\n");
    //call wallet get --> get the amount in the wallet
  
  // Implement this
}

/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {
  // Implement this
  wallet_resource* curr = wallet->head;

  while(curr != NULL) {
    wallet_resource* next = curr->next;
    free(curr);
    curr = next;
  } 

}