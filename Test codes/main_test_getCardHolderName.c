#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  if (getCardHolderName(&tempCard) == OK) {
    printf("Holder Name has been read successfully!.\n");
    printf(" You entered : %s\n", tempCard.cardHolderName);
  } else {
    printf("Wrong Holder Name\n");
  }
}
/*
**** test cases ****
Null
 < 20         Mostafa Ahmed
 > 24         Mostafa Ahmed Abd Elwahab Elfallal
 correct      Mostafa Ahmed Elfallal
*/