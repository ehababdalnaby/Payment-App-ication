#ifndef APP_H_
#define	APP_H_

#include<stdint.h>
#include <stdio.h>
#include <string.h>
#include "..\Card\card.h"
#include "..\Terminal\terminal.h"
#include "..\Server\server.h"

void appStart(void);
/*
* getCardData
* this function recieve the card data from the user
*/
void getCardData(ST_transaction_t* transData);
/*
* pressEnter
* this function waits until the user press enter to continue
*/
void pressEnter(void);

#endif // !APP_H_
