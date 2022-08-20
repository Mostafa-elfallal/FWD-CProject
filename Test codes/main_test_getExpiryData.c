#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  if (getCardExpiryDate(&tempCard) == OK) {
    printf("Expiry Date has been read successfully!.\n");
    printf(" You entered : %d/%d\n", tempCard.cardExpirationDate.month,
           tempCard.cardExpirationDate.year);
  } else {
    printf("Wrong Expiry Date\n");
  }
}
/*
**** test cases ****
 < 5 characters
 > 5 characters
 wrong format
 wrong month number
 correct
*/