
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "..\Card\card.h"
#include "..\Terminal\terminal.h"
#include "server.h"


static uint32_t transactionSequenceNumber = 1;	//this variable holds the last transaction number.
static ST_accountsDB_t currentAccount = { 0,"0" }; //the variable holds the account data that being processed.
EN_transStat_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transStat_t transState = APPROVED;
	//check if the account exist in the database
	if (isValidAccount(&transData->cardHolderData) == DECLINED_STOLEN_CARD)
	{
		printf("Transaction has been declined as this card is stolen\n");
		transState = DECLINED_STOLEN_CARD;
	}
	else if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE)
	{
		printf("Transaction has been declined due to insuffecient fund\n");
		transState = DECLINED_INSUFFECIENT_FUND;
	}
	else if (transState == APPROVED)
	{
		printf("Transaction has been approved\n");
		currentAccount.balance -= transData->terminalData.transAmount;
		updateAccountBalance();
	}
	transData->transState = transState;
	if (saveTransaction(transData) == SAVING_FAILED)
	{
		printf("Transaction hasn't been saved due to an INTERNAL_SERVER_ERROR\n");
		transState = INTERNAL_SERVER_ERROR;
	}
	else
	{
		printTransactionInfo(transData);
	}
	return transState;
}
/*****************************************************************************************************************/
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	EN_serverError_t serverState = DECLINED_STOLEN_CARD;

	if (readAccountInfo(cardData->primaryAccountNumber) == OK_SERVER)
	{
		serverState = OK_SERVER;
	}
	return serverState;

}
/*****************************************************************************************************************/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	EN_serverError_t serverState = LOW_BALANCE;
	if (currentAccount.balance >= termData->transAmount)
	{
		serverState = OK_SERVER;
	}
	return serverState;
}
/*****************************************************************************************************************/
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{

	EN_serverError_t serverState = OK_SERVER;
	if (isFileExist(TRANSACTIONS_DB))
	{
		char newLine = '\n';
		FILE* file = NULL;
		fopen_s(&file, TRANSACTIONS_DB, "a");
		if (file) {
			transData->transactionSequenceNumber = transactionSequenceNumber;
			fwrite(transData, sizeof(ST_transaction_t), 1, file);
			fwrite(&newLine, 1, 1, file);
			transactionSequenceNumber++;
			if (fwrite != 0)
			{

				printf("Transaction has been saved !\n");
			}
			else
			{
				serverState = SAVING_FAILED;
				printf("error writing file !\n");
			}
			fclose(file);
		}
		else
		{
			serverState = SAVING_FAILED;
		}
	}
	else
	{
		serverState = SAVING_FAILED;
	}
	return serverState;
}
/*****************************************************************************************************************/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	EN_serverError_t serverState = TRANSACTION_NOT_FOUND;
	if (isFileExist(TRANSACTIONS_DB))
	{
		FILE* file = NULL;
		char newLine = '\n';
		fopen_s(&file, TRANSACTIONS_DB, "r");
		if (file)
		{
			while (fread(transData, sizeof(ST_transaction_t), 1, file))
			{

				if (transData->transactionSequenceNumber == transactionSequenceNumber)
				{
					serverState = OK_SERVER;
					// close file
					fclose(file);
					break;
				}
				fread(&newLine, 1, 1, file);
			}
		}
		else
		{
			serverState = TRANSACTION_NOT_FOUND;
		}
	}
	else
	{
		serverState = TRANSACTION_NOT_FOUND;
	}


	return serverState;
}
/*****************************************************************************************************************/
void updateAccountBalance(void)
{
	FILE* inputFile = NULL, * outPutFile = NULL;
	char newLine = '\n';
	ST_accountsDB_t Account;
	fopen_s(&inputFile, ACCOUNTS_DB, "r");
	fopen_s(&outPutFile, "temp.text", "w");

	if (inputFile && outPutFile)
	{
		while (fread(&Account, sizeof(ST_accountsDB_t), 1, inputFile))
		{
			fread(&newLine, 1, 1, inputFile);

			if (strcmp(&Account.primaryAccountNumber, &currentAccount.primaryAccountNumber) == 0)
			{
				fwrite(&currentAccount, sizeof(ST_accountsDB_t), 1, outPutFile);
				fwrite(&newLine, 1, 1, outPutFile);
			}
			else
			{
				fwrite(&Account, sizeof(ST_accountsDB_t), 1, outPutFile);
				fwrite(&newLine, 1, 1, outPutFile);
			}

		}
		// close file
		fclose(inputFile);
		fclose(outPutFile);
		remove(ACCOUNTS_DB);
		rename("temp.text", ACCOUNTS_DB);
	}

}
/*****************************************************************************************************************/
EN_serverError_t readAccountInfo(uint8_t* AccountPAN)
{
	EN_serverError_t serverState = ACCOUNT_NOT_FOUND;
	if (isFileExist(ACCOUNTS_DB))
	{
		FILE* file = NULL;
		char newLine = '\n';
		fopen_s(&file, ACCOUNTS_DB, "r");
		if (file)
		{
			while (fread(&currentAccount, sizeof(ST_accountsDB_t), 1, file))
			{
				fread(&newLine, 1, 1, file);
				if (strcmp(&currentAccount.primaryAccountNumber, AccountPAN) == 0)
				{
					serverState = OK_SERVER;
					// close file
					fclose(file);
					break;
				}

			}
		}
	}


	return serverState;
}
/*****************************************************************************************************************/
bool isFileExist(const char* FILENAME)
{
	FILE* file = NULL;
	bool serverState = false;
	fopen_s(&file, FILENAME, "r");
	if (file)
	{
		serverState = true;
		fclose(file);
	}
	return serverState;
}
/*****************************************************************************************************************/
void writeDataToDatabase(void)
{
	FILE* outfile;

	// open file for writing
	fopen_s(&outfile, ACCOUNTS_DB, "w");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nError opened file\n");
	}

	ST_accountsDB_t account1 = { 500,		  "3532329577284122983" };
	ST_accountsDB_t account2 = { 32123.3216,  "4917648756499695" };
	ST_accountsDB_t account3 = { 56512.2698,  "6250941006528599" };
	ST_accountsDB_t account4 = { 62979.5434,  "60115564485789458" };
	ST_accountsDB_t account5 = { 76764.4356,  "6011000991300009" };
	ST_accountsDB_t account6 = { 12563.2135,  "3566000020000410" };
	ST_accountsDB_t account7 = { 16216.2135,  "3530111333300000" };
	ST_accountsDB_t account8 = { 42397.7951,  "5425233430109903" };
	ST_accountsDB_t account9 = { 32163.2135,  "4263982640269299" };
	ST_accountsDB_t account12 = { 39875.3234, "4917484589897107" };
	ST_accountsDB_t account13 = { 43897.2312, "4001919257537193" };
	ST_accountsDB_t account14 = { 49857.3434, "4007702835532454" };
	ST_accountsDB_t account15 = { 24354.6754, "6362970000457013" };
	ST_accountsDB_t account16 = { 23094.0934, "6062826786276634" };
	ST_accountsDB_t account17 = { 87867.3454, "5011054488597827" };
	ST_accountsDB_t account18 = { 93479.9837, "6271701225979642" };
	ST_accountsDB_t account19 = { 32974.3534, "6034932528973614" };
	ST_accountsDB_t account20 = { 43534.7546, "5895626746595650" };
	ST_accountsDB_t account21 = { 45254.3465, "5200533989557118" };
	ST_accountsDB_t account22 = { 59067.3907, "6034883265619896" };
	ST_accountsDB_t account23 = { 30947.5465, "2222420000001113" };
	ST_accountsDB_t account24 = { 43985.6976, "2223000048410010" };

	ST_accountsDB_t arr[] = {
		account1,
		account2,
		account3,
		account4,
		account5,
		account6,
		account7,
		account8,
		account9,
		account12,
		account13,
		account14,
		account15,
		account16,
		account17,
		account18,
		account19,
		account20,
		account21,
		account22,
		account23,
		account24

	};
	// write struct to file
	char newLine = '\n';
	for (int i = 0; i < 22; i++)
	{
		fwrite(&arr[i], sizeof(ST_accountsDB_t), 1, outfile);
		fwrite(&newLine, 1, 1, outfile);
	}
	if (fwrite != 0)
		printf("contents to file written successfully !\n");
	else
		printf("error writing file !\n");
	fclose(outfile);
}
/*****************************************************************************************************************/
EN_serverError_t printAccountInfo(ST_transaction_t* transData)
{
	EN_serverError_t serverState = OK_SERVER;
	if (readAccountInfo(&transData->cardHolderData.primaryAccountNumber) == OK_SERVER)
	{
		printf(SEPERATE_LINE);
		printf("\xB2\xB2\xB2                                     Account info                                       \xB2\xB2\xB2\n");
		printf(SEPERATE_LINE);
		printf("%-30s%-30s%-30s%-30s\n", "Card holder name", "Expiry Date", "PAN", "Balance");
		printf("%-30s%-30s%-30s%-30f\n", transData->cardHolderData.cardHolderName, transData->cardHolderData.cardExpirationDate, transData->cardHolderData.primaryAccountNumber, currentAccount.balance);
	}
	else
	{
		serverState = DECLINED_STOLEN_CARD;
	}
	return serverState;
}
/*****************************************************************************************************************/
EN_serverError_t TransactionIquiry(ST_transaction_t* transData)
{

	EN_serverError_t serverState = OK_SERVER;
	uint32_t transactionSequenceNumber = 0;
	printf("Enter transaction sequence number: ");
	scanf_s("%d", &transactionSequenceNumber);
	scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
	if (getTransaction(transactionSequenceNumber, transData) == OK_SERVER)
	{
		printTransactionInfo(transData);
	}
	else
	{
		printf("This transaction doesn't exist.\n");
		serverState = TRANSACTION_NOT_FOUND;
	}
	return serverState;
}
/*****************************************************************************************************************/
void printTransactionInfo(ST_transaction_t* transData)
{
	char transState[][30] = { "APPROVED","DECLINED INSUFFECIENT FUND","DECLINED STOLEN CARD" };
	printf(SEPERATE_LINE);
	printf("\xB2\xB2\xB2                                     Transaction info                                   \xB2\xB2\xB2\n");
	printf(SEPERATE_LINE);
	printf("%-30s%-30s%-30s\n", "Card holder name", "Expiry Date", "PAN");
	printf("%-30s%-30s%-30s\n", transData->cardHolderData.cardHolderName, transData->cardHolderData.cardExpirationDate, transData->cardHolderData.primaryAccountNumber);
	printf("%-30s%-30s%-30s%-30s\n", "Transaction Date", "Amount", "Sequence number", "Transaction state");
	printf("%-30s%-30f%-30d%-30s\n", transData->terminalData.transactionDate, transData->terminalData.transAmount, transData->transactionSequenceNumber, transState[transData->transState]);

}
/*****************************************************************************************************************/
void updateTransactionSequenceNumber(void)
{
	ST_transaction_t transData;
	char newLine = '\n';
	FILE* file = NULL;
	fopen_s(&file, TRANSACTIONS_DB, "r");

	while (fread(&transData, sizeof(ST_transaction_t), 1, file))
	{
		fread(&newLine, 1, 1, file);
		transactionSequenceNumber++;
	}
	fclose(file);
}