#include "../Server/server.h"

static int getCard(ST_cardData_t *const card) {
  if (getCardHolderName(card) != OK) {
    printf("Invalid Name.\n");
    return 0;
  }
  if (getCardPAN(card) != OK) {
    printf("Invalid PAN.\n");
    return 0;
  }
  if (getCardExpiryDate(card) != OK) {
    printf("Invalid Expiry Date.\n");
    return 0;
  }
  return 1;
}

static int checkTerminal(ST_terminalData_t *const term,
                         ST_cardData_t *const card) {
  getTransactionDate(term);  // no need to check return as it always return OK
  // because it reads OS date
  if (isValidCardPAN(card) != TER_OK) {
    printf("Declined : Invalid PAN.\n");
    return 0;
  }
  if (isCardExpired(card, term) != TER_OK) {
    printf("Declined : Expired Card.\n");
    return 0;
  }
  if (getTransactionAmount(term) != TER_OK) {
    printf("Declined : Wrong Amount.\n");
    return 0;
  }
  if (setMaxAmount(term) != TER_OK) {
    printf("Declined : Terminal Max Amount is wrong.\n");
    return 0;
  }
  if (isBelowMaxAmount(term) != TER_OK) {
    printf("Declined : Amount exceeding limit.\n");
    return 0;
  }
  return 1;
}

int main() {
  ST_cardData_t tempCard;
  ST_terminalData_t tempterm;
  ST_transaction_t tempTrx;
  DB_load();

  if (!getCard(&tempCard)) exit(1);
  if (!checkTerminal(&tempterm, &tempCard)) exit(1);
  tempTrx.cardHolderData = tempCard;
  tempTrx.terminalData = tempterm;

  EN_transState_t retVal = recieveTransactionData(&tempTrx);

  switch (retVal) {
    case DECLINED_STOLEN_CARD:
      printf("DECLINED_STOLEN_CARD\n");
      break;
    case DECLINED_INSUFFECIENT_FUND:
      printf("DECLINED_INSUFFECIENT_FUND\n");
      break;
    case INTERNAL_SERVER_ERROR:
      printf("INTERNAL_SERVER_ERROR \n");
      break;
    default:
      printf("APPROVED\n");
      break;
  }

  DB_store();
}
/*
**** test cases ****
   -> correct trx
  mostafa ahmed elfallal
  2221003372070121
  05/25
  5000

  -> stolen card
  mostafa ahmed elfallal
  6372482079636491
  05/25
  5000

  -> no fund
  mostafa ahmed elfallal
  6381241565763501
  05/25
  5000

  -> internal
  not applicable
*/