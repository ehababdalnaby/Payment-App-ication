
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    EN_cardError_t cardState = OK_CARD;
    char tempName[30] = { '\0' };
    printf(SEPERATE_LINE);
    printf("Card holder name must be 24 characters string max and 20 min.\n");
    printf("Enter card holder name: ");
    fgets(tempName,sizeof(tempName),stdin);
    //clear keyboard buffer 
    if (strchr(tempName, '\n') == NULL) //check if newline doesn't exist then clear the buffer 
    {

        scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
    }
    int inputNameSize = (int)strlen(tempName);
    tempName[inputNameSize - 1] = '\0'; // make the last character = \0 as fgets read until \n
    inputNameSize--;
    if (inputNameSize < MIN_NAME_SIZE || inputNameSize >= MAX_NAME_SIZE)
    {
        cardState = WRONG_NAME;
    }
    else
    {
        strcpy_s(cardData->cardHolderName, MAX_NAME_SIZE,tempName);
    }
    return cardState;
}
/*****************************************************************************************************************/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    EN_cardError_t cardState = OK_CARD;
    uint8_t tempExpiryDate[10] = { '\0' };
    printf(SEPERATE_LINE);
    printf("Card expiry date must be 5 characters string in the format \"MM / YY\", e.g \"05 / 25\"\n");
    printf("Enter card expiry date: ");
    fgets(tempExpiryDate, sizeof(tempExpiryDate), stdin);
    //clear keyboard buffer 
    if (strchr(tempExpiryDate, '\n') == NULL) //check if newline doesn't exist then clear the buffer 
    {

        scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
    }
    int inputExpiryDateSize = (int)strlen(tempExpiryDate);
    tempExpiryDate[inputExpiryDateSize - 1] = '\0';
    inputExpiryDateSize--;
    if (inputExpiryDateSize != 5 || checkExpiryDateFormat(tempExpiryDate) == WRONG_EXP_DATE)
    {
        cardState = WRONG_EXP_DATE;
    }
    else
    {
        strcpy_s(cardData->cardExpirationDate, 6,tempExpiryDate);
    }
    return cardState;
}
/*****************************************************************************************************************/
EN_cardError_t checkExpiryDateFormat(uint8_t *cardExpiryDate)
{
    EN_cardError_t cardState = OK_CARD;
    int mon = atoi(cardExpiryDate);
    int year = atoi(&cardExpiryDate[3]);
    if (mon == 0 || mon > 12 || cardExpiryDate[2] != '/' || year == 0)
    {
        cardState = WRONG_EXP_DATE;
    }
    return cardState;
}
/*****************************************************************************************************************/
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    EN_cardError_t cardState = OK_CARD;
    uint8_t tempCardPAN[30] = { '\0' };
    printf(SEPERATE_LINE);
    printf("PAN must be 20 characters alphanumeric only string 19 character max, and 16 character min\n");
    printf("Enter the card's Primary Account Number: ");
    fgets(tempCardPAN, sizeof(tempCardPAN), stdin);
    //clear keyboard buffer 
    if (strchr(tempCardPAN, '\n') == NULL) //check if newline doesn't exist then clear the buffer 
    {

        scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
    }
    int inputCardPANSize = (int)strlen(tempCardPAN);
    tempCardPAN[inputCardPANSize - 1] = '\0';
    inputCardPANSize--;
    if (inputCardPANSize < MIN_ACCOUNT_SIZE || inputCardPANSize >= MAX_ACCOUNT_SIZE || checkCardPAN(tempCardPAN) == WRONG_PAN)
    {
        cardState = WRONG_PAN;
    }
    else
    {
        strcpy_s(cardData->primaryAccountNumber, MAX_ACCOUNT_SIZE, tempCardPAN);
    }

    return cardState;
}
/*****************************************************************************************************************/
EN_cardError_t checkCardPAN(uint8_t* cardPAN)
{
    EN_cardError_t cardState = OK_CARD;
    int i = 0;
    while (cardPAN[i] != '\0')
    {
        if (cardPAN[i] < '0' || cardPAN[i] > '9')
        {
            printf("PAN must contain only numbers 0-9.\n");
            cardState = WRONG_PAN;
            break;
        }
        i++;
    }
    return cardState;
}
