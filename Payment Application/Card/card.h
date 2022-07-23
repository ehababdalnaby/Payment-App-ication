#ifndef CARD_H_
#define CARD_H_


#define SEPERATE_LINE "\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n"

#define MAX_NAME_SIZE 25
#define MIN_NAME_SIZE 20
#define MAX_ACCOUNT_SIZE 20
#define MIN_ACCOUNT_SIZE 16
#define EXPIRATION_DATE_SIZE 6

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[MAX_NAME_SIZE];
	uint8_t primaryAccountNumber[MAX_ACCOUNT_SIZE];
	uint8_t cardExpirationDate[EXPIRATION_DATE_SIZE];
} ST_cardData_t;

typedef enum EN_cardError_t
{
	OK_CARD,
	WRONG_NAME,
	WRONG_EXP_DATE,
	WRONG_PAN
} EN_cardError_t;

/*
 * getCardHolderName
 *1.This function will ask for the cardholder's name and store it into card data.
 *2.Card holder name is 24 characters string max and 20 min.
 *3.If the cardholder name is NULL, less than 20 characters or more than 24 will return WRONG_NAME error, else return OK.
 */
EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
/*
 * getCardExpiryDate
 *1.This function will ask for the card expiry date and store it in card data.
 *2.Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
 *3.If the card expiry date is NULL, less or more than 5 characters, or has the wrong format will return WRONG_EXP_DATE error, else return OK.
 */
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
/*
 * getCardPAN
 *1.This function will ask for the card's Primary Account Number and store it in card data.
 *2.PAN is 20 characters alphanumeric only string 19 character max, and 16 character min.
 *3.If the PAN is NULL, less than 16 or more than 19 characters, will return WRONG_PAN error, else return OK.
 */
EN_cardError_t getCardPAN(ST_cardData_t *cardData);
/*
 * checkCardPAN
 *1.This function will check if the card's Primary Account Number is alphanumeric only.
 *2.If the PAN is not alphanumeric, will return WRONG_PAN error, else return OK.
 */
EN_cardError_t checkCardPAN(uint8_t *cardPAN);
/*
 * checkCardPAN
 *1.This function will check if the card's Expiry date format is correct e.g "07/22".
 *2.If the format is not correct, will return WRONG_EXP_DATE error, else return OK.
 */
EN_cardError_t checkExpiryDateFormat(uint8_t *cardExpiryDate);

#endif // !CARD_H_ 