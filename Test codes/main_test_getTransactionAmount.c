#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  ST_terminalData_t tempterm;
  if (getTransactionAmount(&tempterm) != TER_OK)
    printf("wrong amount.\n");
  else
    printf("acceptable amount.\n");
}
/*
**** test cases ****
 -1000
 0
 1000
 20000
*/