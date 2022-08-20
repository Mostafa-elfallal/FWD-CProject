#include "../Server/server.h"
int main() {
  ST_cardData_t tempCard;
  ST_terminalData_t tempterm;
  if (getTransactionDate(&tempterm) == TER_OK) {
    printf("Transaction Date has been read successfully!.\n");
    printf(" You entered : %d/%d/%d\n", tempterm.transactionDate.day,
           tempterm.transactionDate.month, tempterm.transactionDate.year);
  } else {
    printf("Wrong Date\n");
  }
}
/*
**** test cases ****
No test cases as it reads windows time
 correct
*/