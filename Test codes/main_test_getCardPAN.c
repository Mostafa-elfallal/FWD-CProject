#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  if (getCardPAN(&tempCard) == OK) {
    printf("PAN has been read successfully!.\n");
    printf(" You entered : %s\n", tempCard.primaryAccountNumber);
  } else {
    printf("Wrong PAN\n");
  }
}
/*
**** test cases ****
Null
 < 16         123456789101123
 > 19         123456789101112131415
 correct      1234567891234567891
*/