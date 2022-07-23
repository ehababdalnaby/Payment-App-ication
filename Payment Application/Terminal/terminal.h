#ifndef TERMINAL_H_
#define TERMINAL_H_

#define DATE_SIZE 11
typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[DATE_SIZE];
} ST_terminalData_t;

typedef enum EN_terminalError_t
{
	OK_TERMINAL,
	WRONG_DATE,
	EXPIRED_CARD,
	INVALID_CARD,
	INVALID_AMOUNT,
	EXCEED_MAX_AMOUNT,
	INVALID_MAX_AMOUNT
} EN_terminalError_t;

/*
 *getTransactionDate
 *1.This function will ask for the transaction data and store it in terminal data.
 *2.Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
 *3.If the transaction date is NULL, less than 10 characters or wrong format will return WRONG_DATE error, else return OK.
 *Optional:
 *The function will read the current date from your computer and store it into terminal data with the mentioned size and format.
 */
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
/*
 *isCardExpired
 *1.This function compares the card expiry date with the transaction date.
 *2.If the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.
 */
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
/*
 *getTransactionAmount
 *1.This function asks for the transaction amount and saves it into terminal data.
 *2.If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return OK.
 */
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
/*
 *isBelowMaxAmount
 *1.This function compares the transaction amount with the terminal max amount.
 *2.If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT, else return OK.
 */
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
/*
 *setMaxAmount
 *1.This function sets the maximum allowed amount into terminal data.
 *2.Transaction max amount is a float number.
 *3.If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error, else return OK.
 */
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData);
/*
 *isValidCardPAN
 *1.This function checks if the PAN is a Luhn number or not.
 *2.If PAN is not a Luhn number will return INVALID_CARD, else return OK.
 */
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);

#endif // !TERMINAL_H_
