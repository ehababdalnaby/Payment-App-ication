
#include <stdint.h>
#include "..\Card\card.h"
#include "terminal.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define Size 11

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    EN_terminalError_t terminalState = OK_TERMINAL;
    time_t t;
    struct tm tmp;
    char MY_TIME[Size];
    time(&t); //read number of seconds since January 1, 1970.

    localtime_s(&tmp,&t);//convert sconds to days,months and years. 

    // using strftime to display time in required format.
    strftime(MY_TIME, sizeof(MY_TIME), "%d/%m/%Y", &tmp);

    strcpy_s(termData->transactionDate, Size, MY_TIME);
    return terminalState;
}
/*****************************************************************************************************************/
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)

{
    EN_terminalError_t terminalState = EXPIRED_CARD;
  
    if (strncmp(&termData.transactionDate[8],&cardData.cardExpirationDate[3], 2) < 0) //if the current year is less than the card year then this cared is valid
    {
        terminalState = OK_TERMINAL;
    }
    else if (strncmp(&termData.transactionDate[8], &cardData.cardExpirationDate[3], 2) == 0 && strncmp(&termData.transactionDate[3], cardData.cardExpirationDate, 2) <= 0)
    {
        terminalState = OK_TERMINAL;
    }
    return terminalState;
}
/*****************************************************************************************************************/
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    /*
    * to check if the PAN is a luhn number or not.
    * 1.multiply odd or even indecies by two.
    * 2.if there is number consist of two digits sum them.
    * 3.sum all the number if the sum is divisible by 10 then this is a luhn number.
    * e.g 1 2 3 6 5  2 1 3 6  9 5  8 2 3 6  5
    *     2 2 6 6 10 2 2 3 12 9 10 8 4 3 12 5
    *     2 2 6 6 1  2 2 3 3  9 1  8 4 3 3  5
    *     sum = 60
    */
    EN_terminalError_t terminalState = INVALID_CARD;
    int sum = 0;
    int oddSum = 0;
    int cuurrentAccountDigit = 0;
    int temp = 0;
    for(int i = 0; cardData->primaryAccountNumber[i] != '\0'; i++)
    {
        cuurrentAccountDigit = cardData->primaryAccountNumber[i] - '0';
        int oddDigit = cuurrentAccountDigit;
        if(i%2 == 0)
        {
            cuurrentAccountDigit *= 2;
        }
        else
        {
            oddDigit *= 2;
        }
        temp = cuurrentAccountDigit/10;
        temp += cuurrentAccountDigit%10;
        sum+=temp;

        temp = oddDigit / 10;
        temp += oddDigit % 10;
        oddSum += temp;
    }
    if(sum%10 == 0 || oddSum%10 == 0)
    {
        terminalState = OK_TERMINAL;
    }
    return terminalState;
}
/*****************************************************************************************************************/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    EN_terminalError_t terminalState = OK_TERMINAL;
    printf(SEPERATE_LINE);
    printf("Transaction amount must be less than or equal to %f .\n", termData->maxTransAmount);
    printf("Enter transaction amount: ");
    scanf_s("%f", &termData->transAmount);
    scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
    if (termData->transAmount <= 0)
    {
        printf("Transaction amount must be larger than 0\n");
        terminalState = INVALID_AMOUNT;
    }
    return terminalState;
}
/*****************************************************************************************************************/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    EN_terminalError_t terminalState = OK_TERMINAL;
    if (termData->transAmount > termData->maxTransAmount)
    {
        printf("Excedded Max amount.\n");
        terminalState = EXCEED_MAX_AMOUNT;
    }
    return terminalState;
}
/*****************************************************************************************************************/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    EN_terminalError_t terminalState = OK_TERMINAL;
    printf(SEPERATE_LINE);
    printf("Max amount must be > 0\n");
    printf("Enter Max transaction amount allowed: ");
    scanf_s("%f", &termData->maxTransAmount);
    scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
    if (termData->maxTransAmount <= 0)
    {
        printf("Invalid Max amount.\n");
        terminalState = INVALID_MAX_AMOUNT;
    }
    return terminalState;
}
