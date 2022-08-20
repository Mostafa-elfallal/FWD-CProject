#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  ST_terminalData_t tempterm;
  (void)setMaxAmount(&tempterm);  // reads from a file, skip checking
                                  // max is set to 10K
  if (getTransactionAmount(&tempterm) != TER_OK) printf("wrong amount.\n");
  if (isBelowMaxAmount(&tempterm) == TER_OK) {
    printf("Below Maximum amount.\n");
  } else {
    printf("Above Maximum amount.\n");
  }
}
/*
**** test cases ****
 5000
 10000
 15000
*/