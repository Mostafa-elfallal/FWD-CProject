#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  ST_terminalData_t tempterm;

  if (getCardExpiryDate(&tempCard) == OK) {
    /*
    printf("Expiry Date has been read successfully!.\n");
    printf(" You entered : %d/%d\n", tempCard.cardExpirationDate.month,
           tempCard.cardExpirationDate.year);
    */
  } else {
    printf("Wrong Expiry Date\n");
  }

  if (getTransactionDate(&tempterm) == TER_OK) {
    /*
    printf("Transaction Date has been read successfully!.\n");
    printf(" You entered : %d/%d/%d\n", tempterm.transactionDate.day,
           tempterm.transactionDate.month, tempterm.transactionDate.year);*/
  } else {
    printf("Wrong Date\n");
  }

  if (isCardExpired(&tempCard, &tempterm) == TER_OK) {
    printf("Card is not expired.\n");
  } else {
    printf("Expired Card.\n");
  }
}
/*
**** test cases ****
  11/15
 07/22
 08/22
 09/22
 01/25
*/