#include "terminal.h"
#include "time.h"
#include "string.h"
EN_terminalError_t getTransactionDate(ST_terminalData_t *const termData) {
  /*
      This function will ask for the transaction data and store it in terminal
     data.
      Transaction date is 10 characters string in the format DD/MM/YYYY, e.g
     25/06/2022.
      If the transaction date is NULL, less than 10 characters or wrong format
     will return WRONG_DATE error, else return OK.
  */
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  termData->transactionDate.day = tm.tm_mday;
  termData->transactionDate.month = tm.tm_mon + 1;
  termData->transactionDate.year = tm.tm_year + 1900;
  return TER_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t const *const cardData,
                                 ST_terminalData_t const *const termData) {
  /*
      This function compares the card expiry date with the transaction date.
      If the card expiration date is before the transaction date will return
     EXPIRED_CARD, else return OK.
  */
  if (cardData->cardExpirationDate.year > termData->transactionDate.year)
    return TER_OK;
  if ((cardData->cardExpirationDate.year == termData->transactionDate.year) &&
      (cardData->cardExpirationDate.month >= termData->transactionDate.month))
    return TER_OK;
  return EXPIRED_CARD;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t const *const cardData) {
  /*
      Starting from the rightmost digit, double the value of every second digit
      If doubling of a number results in a two digit number , then add the
     digits of the product
      Now take the sum of all the digits
      If the total modulo 10 is equal to 0 then the number is valid
  */
  // to perform these operation easily i'll store the chars as ints
  int cardPANLen = strlen(cardData->primaryAccountNumber);
  int *PANarray = malloc(cardPANLen * sizeof(int));
  if (PANarray == NULL) {
    printf("Couldn't reserve memory for PAN integer array\n");
    exit(1);
  }
  for (int i = 0; i < cardPANLen; i++) {
    PANarray[i] = (int)(cardData->primaryAccountNumber[i] - '0');
  }

  int sum = 0;
  for (int i = cardPANLen - 2; i >= 0; i -= 2) {
    PANarray[i] *= 2;         // double
    if (PANarray[i] >= 10) {  // add the two digits if it's greater than 9
      PANarray[i] -= 10;
      PANarray[i] += 1;
    }
  }
  for (int i = 0; i < cardPANLen; i++) {
    sum += PANarray[i];
  }
  if (sum % 10 == 0)
    return TER_OK;
  else
    return INVALID_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *const termData) {
  /*
      This function asks for the transaction amount and saves it into terminal
     data.
      If the transaction amount is less than or equal to 0 will return
     INVALID_AMOUNT, else return OK.
  */
  printf("Enter the transaction amount : ");
  scanf("%f", &(termData->transAmount));
  if (termData->transAmount <= 0) return INVALID_AMOUNT;
  return TER_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t const *const termData) {
  /*
      This function compares the transaction amount with the terminal max
     amount.
      If the transaction amount is larger than the terminal max amount will
     return EXCEED_MAX_AMOUNT, else return OK.
  */
  return termData->transAmount > termData->maxTransAmount ? EXCEED_MAX_AMOUNT
                                                          : TER_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *const termData) {
  /*
      This function sets the maximum allowed amount into terminal data.
      Transaction max amount is a float number.
      If transaction max amount less than or equal to 0 will return
     INVALID_MAX_AMOUNT error, else return OK.
  */
  // this will read the maximum amount from a txt file
  FILE *fptr = fopen("Files/Maxamount.txt", "r");
  fscanf(fptr, " %f ", &(termData->maxTransAmount));
  fclose(fptr);
  if (termData->maxTransAmount <= 0) return INVALID_MAX_AMOUNT;

  return TER_OK;
}