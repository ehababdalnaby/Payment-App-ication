#include "app.h"

void appStart(void)
{
	ST_transaction_t transData;
	EN_serverError_t serverstate = OK_SERVER;

	EN_terminalError_t terminalState = OK_TERMINAL;
	updateTransactionSequenceNumber(); //update the transaction sequence number with the last number reached in the datatbase
	do
	{
		//set the max allowed amount.
		terminalState = setMaxAmount(&transData.terminalData);
	} while (terminalState == INVALID_MAX_AMOUNT);

	while (true)
	{

		int choice = 0;
		getCardData(&transData);
		do
		{
			system("cls");
			printf(SEPERATE_LINE);
			printf("\xB2\xB2\xB2                                      1.Withdrwal                                       \xB2\xB2\xB2\n");
			printf("\xB2\xB2\xB2                                      2.Balance inquiry                                 \xB2\xB2\xB2\n");
			printf("\xB2\xB2\xB2                                      3.Transaction inquiry                             \xB2\xB2\xB2\n");
			printf("\xB2\xB2\xB2                                      4.Quit                                            \xB2\xB2\xB2\n");
			printf(SEPERATE_LINE);
			printf("Enter your choice: ");
			scanf_s("%d", &choice);
			scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
			switch (choice)
			{
			case 1:
				printAccountInfo(&transData);
				do
				{
					do
					{
						//get the transaction amount
						terminalState = getTransactionAmount(&transData.terminalData);
					} while (terminalState == INVALID_AMOUNT);
					//check if the transaction amount is larger than max amount.
					terminalState = isBelowMaxAmount(&transData.terminalData);
				} while (terminalState == EXCEED_MAX_AMOUNT);

				serverstate = recieveTransactionData(&transData);
				break;
			case 2:
				serverstate = printAccountInfo(&transData);
				if (serverstate == DECLINED_STOLEN_CARD)
				{
					printf("!!!This card is stolen!!!\n");
				}
				break;
			case 3:
				serverstate = TransactionIquiry(&transData);
				break;
			case 4:
				printf("Quit...\n");
				break;
			default:
				printf("Wrong choice\n");
				break;
			}
			pressEnter();
		} while (choice != 4);

	}
}


void getCardData(ST_transaction_t* transData)
{
	EN_terminalError_t terminalState = OK_TERMINAL;
	EN_cardError_t cardState = OK_CARD;
	system("cls");
	do
	{
		do
		{
			printf(SEPERATE_LINE);
			printf("\xB2\xB2\xB2                                     Get card data                                      \xB2\xB2\xB2\n");

			//get card holder name
			do
			{

				cardState = getCardHolderName(&transData->cardHolderData);
			} while (cardState == WRONG_NAME);
			//get card expiry date
			do
			{

				cardState = getCardExpiryDate(&transData->cardHolderData);
			} while (cardState == WRONG_EXP_DATE);
			//get card PAN
			do
			{

				cardState = getCardPAN(&transData->cardHolderData);
			} while (cardState == WRONG_PAN);

			//get transaction date from the computer.
			getTransactionDate(&transData->terminalData);

			//check if the card expired or not.
			terminalState = isCardExpired(transData->cardHolderData, transData->terminalData);
			if (terminalState == EXPIRED_CARD)
			{
				printf("!!!This card is expired!!!\n");
				pressEnter();
			}
		} while (terminalState == EXPIRED_CARD);
		//check if the PAN is a luhn number or not.
		terminalState = isValidCardPAN(&transData->cardHolderData);
		if (terminalState == INVALID_CARD)
		{
			printf("!!!This card is not valid!!!\n");
			pressEnter();
		}
	} while (terminalState == INVALID_CARD);
}



void pressEnter(void)
{
	printf("Press enter to continue....\n");
	scanf_s("%*[^\n]"); scanf_s("%*c");//clear upto newline
	system("cls");
}
