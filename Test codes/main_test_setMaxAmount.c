#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  ST_terminalData_t tempterm;
  if (setMaxAmount(&tempterm) != TER_OK)
    // reads from file as it is not a user data
    printf("wrong Max amount.\n");
  else
    printf("acceptable Max amount.\n");
}
/*
**** test cases ****
 -1000
 0
 1000
 20000
*/