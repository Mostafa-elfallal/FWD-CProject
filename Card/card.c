#include "card.h"
#include "stdio.h"
#include "string.h"
#include <ctype.h>
EN_cardError_t getCardHolderName(ST_cardData_t *const cardData) {
  /*
      This function will ask for the cardholder's name and store it into card
     data.
      Card holder name is 24 characters string max and 20 min.
      If the cardholder name is NULL, less than 20 characters or more than 24
     will return WRONG_NAME error, else return OK.
  */
  uint8_t buffer[MAX_BUFFER_LEN];
  printf("Enter Name on Card : ");
  fflush(stdin);
  gets(buffer);
  uint8_t cardNameLen = strlen(buffer);
  if (cardNameLen < MIN_NAME_LEN || cardNameLen > MAX_NAME_LEN)
    return WRONG_NAME;
  strcpy(cardData->cardHolderName, buffer);
  cardData->cardHolderName[cardNameLen] = '\0';
  return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *const cardData) {
  /*
        This function will ask for the card expiry date and store it in card
    data.
        Card expiry date is 5 characters string in the format "MM/YY", e.g
    "05/25".
        If the card expiry date is NULL, less or more than 5 characters, or has
    the wrong format will return WRONG_EXP_DATE error, else return OK.
  */
  uint8_t month, year;
  uint8_t buffer[MAX_BUFFER_LEN];
  printf("Enter Expiry Date (MM/YY) : ");
  gets(buffer);
  uint8_t cardExpiryLen = strlen(buffer);
  if (cardExpiryLen != 5) return WRONG_EXP_DATE;
  if (buffer[2] != '/') return WRONG_EXP_DATE;
  if (!(isdigit(buffer[0]) && isdigit(buffer[1]) && isdigit(buffer[3]) &&
        isdigit(buffer[4])))
    return WRONG_EXP_DATE;
  month = (buffer[0] - '0') * 10 + (buffer[1] - '0');
  year = (buffer[3] - '0') * 10 + (buffer[4] - '0');
  // printf("%s\n", buffer);
  // printf("%d %d\n", month, year);
  if (month > 12 || month < 1) return WRONG_EXP_DATE;

  cardData->cardExpirationDate.month = month;
  cardData->cardExpirationDate.year = year + 2000;
  return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *const cardData) {
  /*
      This function will ask for the card's Primary Account Number and store it
     in card data.
      PAN is 20 characters alphanumeric only string 19 character max, and 16
     character min.
      If the PAN is NULL, less than 16 or more than 19 characters, will return
     WRONG_PAN error, else return OK.
  */
  uint8_t buffer[MAX_BUFFER_LEN];
  printf("Enter PAN : ");
  gets(buffer);
  uint8_t cardPANLen = strlen(buffer);
  if (cardPANLen < MIN_NUM_LEN || cardPANLen > MAX_NUM_LEN) return WRONG_PAN;
  strcpy(cardData->primaryAccountNumber, buffer);
  cardData->primaryAccountNumber[cardPANLen] = '\0';
  return OK;
}