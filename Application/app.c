#include "app.h"
#include "stdio.h"
#define MAX_PATH_LEN 20
#define SALE 1
#define GETTRANS 2
#define EXIT 3
static void welcomeScreen(void) { printf("Welcome to our payment system,\n"); }
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
static void checkServer(ST_transaction_t *const trx) {
  trx->transState = recieveTransactionData(trx);
  if (saveTransaction(trx) == SAVING_FAILED) printf("Saving Failed !!!!\n");
  switch (trx->transState) {
    case DECLINED_STOLEN_CARD:
      printf("Declined : Stolen Card.\n");
      break;
    case DECLINED_INSUFFECIENT_FUND:
      printf("Declined : Insuffeciant Fund.\n");
      break;
    case INTERNAL_SERVER_ERROR:
      printf("Declined : Internal Server Error.\n");
      break;
    default:
      printf("Approved\n");
  }
}
static void displayTransaction(ST_transaction_t const *const Trx) {
  char state[50];
  printf("Sequence Number : %d\n", Trx->transactionSequenceNumber);
  printf("***\tCard Data\t***\n");
  printf("PAN : %s\n", Trx->cardHolderData.primaryAccountNumber);
  printf("Name : %s\n", Trx->cardHolderData.cardHolderName);
  printf("Expiry Date : %02d/%02d\n",
         Trx->cardHolderData.cardExpirationDate.month,
         Trx->cardHolderData.cardExpirationDate.year);
  printf("***\tTerminal Data\t***\n");
  printf("Max Amount : %.2f\n", Trx->terminalData.maxTransAmount);
  printf("***\tTransaction Data\t***\n");
  printf("Amount : %.2f\n", Trx->terminalData.transAmount);
  printf("Date : %02d/%02d/%4d\n", Trx->terminalData.transactionDate.day,
         Trx->terminalData.transactionDate.month,
         Trx->terminalData.transactionDate.year);
  switch (Trx->transState) {
    case DECLINED_STOLEN_CARD:
      strcpy(state, "Declined : Stolen Card.");
      break;
    case DECLINED_INSUFFECIENT_FUND:
      strcpy(state, "Declined : Insuffeciant Fund.");
      break;
    case INTERNAL_SERVER_ERROR:
      strcpy(state, "Declined : Internal Server Error.");
      break;
    default:
      strcpy(state, "Approved.");
  }
  printf("State : %s\n", state);
}
void appStart(void) {
  ST_cardData_t card;
  ST_terminalData_t Terminal;
  ST_transaction_t transaction;
  int choice;
  uint32_t seqNum;
  while (1) {
    printf("-----------------------------------------------------------\n");
    welcomeScreen();
    printf(
        "Choose the operation you want to do : \n1 - SALE \n2 - Get old "
        "Operation\n3 - Exit\n");
    scanf("%d", &choice);
    // *************** SALE *******************
    if (choice == SALE) {
      if (!getCard(&card)) {
        continue;  // failed to get card
      }
      if (!checkTerminal(&Terminal, &card)) {
        continue;  // Terminal checks didn't complete
      }
      transaction.cardHolderData = card;
      transaction.terminalData = Terminal;
      checkServer(&transaction);
      // *************** get Transaction *******************
    } else if (choice == GETTRANS) {
      printf("Enter the sequence number :\n");
      scanf("%d", &seqNum);
      if (getTransaction(seqNum, &transaction) == SERVER_OK) {
        printf("Transaction Details :\n");
        displayTransaction(&transaction);
      } else {
        printf("Transaction is not found.\n");
      }
      // *************** Exit *******************
    } else if (choice == EXIT) {
      break;
      // *************** else *******************
    } else {
      printf("Invalid Choice.\n");
      continue;
    }
  }
}
