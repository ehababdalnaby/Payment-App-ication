#ifndef SERVER_H_
#define SERVER_H_

#include <stdbool.h>

#define ACCOUNTS_DB		"Accounts DB.txt"
#define TRANSACTIONS_DB "Transactions DB.txt"

typedef enum EN_transState_t
{
	APPROVED,
	DECLINED_INSUFFECIENT_FUND,
	DECLINED_STOLEN_CARD,
	INTERNAL_SERVER_ERROR
} EN_transStat_t;

typedef enum EN_serverError_t
{
	OK_SERVER,
	SAVING_FAILED,
	TRANSACTION_NOT_FOUND,
	ACCOUNT_NOT_FOUND,
	LOW_BALANCE
} EN_serverError_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transStat_t transState;
	uint32_t transactionSequenceNumber;
} ST_transaction_t;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
} ST_accountsDB_t;

/*
*recieveTransactionData
*1.1This function will take all transaction data and validate its data.
*2.1It checks the account details and amount availability.
*3.1If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available
	will return DECLINED_INSUFFECIENT_FUND, if a transaction can't be saved will return INTERNAL_SERVER_ERROR
	and will not save the transaction, else returns APPROVED.
*4.1It will update the database with the new balance.
*/
EN_transStat_t recieveTransactionData(ST_transaction_t *transData);
/*
 *isValidAccount
 *1.This function will take card data and validate these data.
 *2.It checks if the PAN exists or not in the server's database.
 *3.If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK
 */
EN_serverError_t isValidAccount(ST_cardData_t *cardData);
/*
 *isAmountAvailable
 *1.This function will take terminal data and validate these data.
 *2.It checks if the transaction's amount is available or not.
 *3.If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return OK
 */
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData);
/*
 *saveTransaction
 *1.This function will take all transaction data into the transactions database.
 *2.It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server.
 *3.If saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
 *4.If transaction can't be saved will return SAVING_FAILED, else will return OK
 */
EN_serverError_t saveTransaction(ST_transaction_t *transData);
/*
*getTransaction
*1.This function will take a transaction sequence number and search for this transaction in the database.
*2.If the transaction can't be found will return TRANSACTION_NOT_FOUND, else will return OK and store the
transaction in a structure
*/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData);
/*
* updateAccountBalance
* this function updates the database with the new balance after a successful transaction.
*/
void updateAccountBalance(void);
/*
* readAccountInfo
* 1.this function will take the account PAN and search for it in the database.
* 2.this function read the wanted account data and store this data in a static global variable currentAccount.
* 3.if the account doesn't exist will return ACCOUNT_NOT_FOUND else return OK.
*/
EN_serverError_t readAccountInfo(uint8_t* AccountPAN);
/*
* isFileExist
* 1.this function takes the file name.
* 2.if the file exist will return true else return false.
*/
bool isFileExist(const char* FILENAME);
/*
* writeDataToDatabase
* 1.this function will write the database to the file Accounts DB.txt.
*/
void writeDataToDatabase(void);
/*
* TransactionIquiry
* this function takes transsaction Data and search for the transaction info in database.
* ig the transaction exist it prints it to the user else return error.
*/
EN_serverError_t TransactionIquiry(ST_transaction_t* transData);
/*
* printTransactionInfo
* this function print the transaction info to the user in a formated way.
*/
void printTransactionInfo(ST_transaction_t* transData);
/*
* printAccountInfo
* this function print the account info to the user in a formated way.
*/
EN_serverError_t printAccountInfo(ST_transaction_t* transData);
/*
* updateTransactionSequenceNumber
* this function updates the transaction sequence number with the last number in the database.
*/
void updateTransactionSequenceNumber(void);
#endif // !SERVER_H_								   

