#include "server.h"
#include <ctype.h>
#define BUFFER_SIZE 255
/*
    Implement server-side accounts' database

1- Create a global array of ST_accountsDB_t for the valid accounts database
2- Fill in the array initially with any valid data
3- This array has a maximum of 255 element/account data
*/
static ST_accountsDB_t Accounts_DB[BUFFER_SIZE];
static uint8_t Accounts_DB_top = 0;
// will be loaded at the start of the prgram from a file and will be stored at
// the end
/*
    Implement server-side transactions' database

1- Create a global array of ST_transaction_t
2- Fill in the array initially with 0s.
3- This array has a maximum of 255 element/transaction data
*/
static ST_transaction_t Transctions_DB[BUFFER_SIZE] = {0};
static uint8_t Transctions_DB_top = 0;

static void getNameIndeces(char *const buffer, int *const start, int *const end,
                           int *const numstart) {
  int state = 0;
  for (int i = 0; buffer[i] != '\0'; i++) {
    switch (state) {
      case 0:
        if (isalpha(buffer[i])) {
          *start = i;
          state = 1;
        }
        break;
      case 1:
        if (isalpha(buffer[i])) {
          *end = i;
        } else if (isdigit(buffer[i])) {
          *numstart = i;
          state = 2;
        }
        break;
      default:
        break;
    }
  }
}
/*
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
*/
void DB_load(void) {
  // load the accounts DB file into the accounts DB array
  ST_transaction_t transTemp;
  int nameStartIndex, nameEndIndex, numStartIndex;
  FILE *fPtr = fopen("Files/Accounts DB.txt", "r");
  char buffer[BUFFER_SIZE];
  while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL) {
    sscanf(buffer, "%s %f",
           &(Accounts_DB[Accounts_DB_top].primaryAccountNumber),
           &(Accounts_DB[Accounts_DB_top].balance));
    Accounts_DB_top++;
  }
  fclose(fPtr);
  fPtr = fopen("Files/Transactions DB.txt", "r");
  while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL) {
    sscanf(buffer, "%d\t%s\t%s\t%02d/%4d\t%f\t%f\t%d/%d/%d\t%d\n",
           // strings are read wrong in this context
           &(transTemp.transactionSequenceNumber),
           transTemp.cardHolderData.primaryAccountNumber,
           transTemp.cardHolderData.cardHolderName,
           &(transTemp.cardHolderData.cardExpirationDate.month),
           &(transTemp.cardHolderData.cardExpirationDate.year),
           &(transTemp.terminalData.maxTransAmount),
           &(transTemp.terminalData.transAmount),
           &(transTemp.terminalData.transactionDate.day),
           &(transTemp.terminalData.transactionDate.month),
           &(transTemp.terminalData.transactionDate.year),
           &(transTemp.transState));
    // the right account
    getNameIndeces(buffer, &nameStartIndex, &nameEndIndex, &numStartIndex);
    // correcting names
    strncpy(transTemp.cardHolderData.cardHolderName, buffer + nameStartIndex,
            nameEndIndex - nameStartIndex + 1);
    transTemp.cardHolderData.cardHolderName[nameEndIndex - nameStartIndex + 1] =
        '\0';
    sscanf(buffer + numStartIndex, "%d/%d\t%f\t%f\t%d/%d/%d\t%d\n",
           &(transTemp.cardHolderData.cardExpirationDate.month),
           &(transTemp.cardHolderData.cardExpirationDate.year),
           &(transTemp.terminalData.maxTransAmount),
           &(transTemp.terminalData.transAmount),
           &(transTemp.terminalData.transactionDate.day),
           &(transTemp.terminalData.transactionDate.month),
           &(transTemp.terminalData.transactionDate.year),
           &(transTemp.transState));
    Transctions_DB[Transctions_DB_top++] = transTemp;
  } /*
   for (int i = 0; i < Accounts_DB_top; i++) {
     printf("PAN : %s balance : %f\n", Accounts_DB[i].primaryAccountNumber,
            Accounts_DB[i].balance);
   }
   */
  /*
  for (int i = 0; i < Transctions_DB_top; i++) {
    displayTransaction(&Transctions_DB[i]);
  }
  */
}

void DB_store(void) {
  FILE *fptr = fopen("Files/Transactions DB.txt", "w");
  ST_transaction_t *transData = Transctions_DB;
  for (uint8_t i = 0; i < Transctions_DB_top; i++) {
    fprintf(fptr, "%d\t%s\t%s\t%02d/%02d\t%.2f\t%.2f\t%02d/%02d/%4d\t%d\n",
            transData->transactionSequenceNumber,
            transData->cardHolderData.primaryAccountNumber,
            transData->cardHolderData.cardHolderName,
            transData->cardHolderData.cardExpirationDate.month,
            transData->cardHolderData.cardExpirationDate.year,
            transData->terminalData.maxTransAmount,
            transData->terminalData.transAmount,
            transData->terminalData.transactionDate.day,
            transData->terminalData.transactionDate.month,
            transData->terminalData.transactionDate.year,
            transData->transState);
    transData++;
  }
  fclose(fptr);
  fptr = fopen("Files/Accounts DB.txt", "w");
  ST_accountsDB_t *accountData = Accounts_DB;
  for (uint8_t i = 0; i < Accounts_DB_top; i++) {
    fprintf(fptr, "%s\t%f\n", accountData->primaryAccountNumber,
            accountData->balance);
    accountData++;
  }
  fclose(fptr);
}
static EN_serverError_t updateAccountBalance(
    ST_terminalData_t const *const termData, const int index) {
  Accounts_DB[index].balance -= termData->transAmount;
  return SERVER_OK;
}

EN_transState_t recieveTransactionData(ST_transaction_t *const transData) {
  /*
      This function will take all transaction data and validate its data.
      It checks the account details and amount availability.
      If the account does not exist return DECLINED_STOLEN_CARD, if the amount
     is not available will return DECLINED_INSUFFECIENT_FUND, if a transaction
     can't be saved will return INTERNAL_SERVER_ERROR and will not save the
     transaction, else returns APPROVED.
      It will update the database with the new balance.
  */
  int accountLoc;
  EN_serverError_t retVal =
      isValidAccount(&(transData->cardHolderData), &accountLoc);
  if (retVal != SERVER_OK) return DECLINED_STOLEN_CARD;

  retVal = isAmountAvailable(&(transData->terminalData), accountLoc);
  if (retVal != SERVER_OK) return DECLINED_INSUFFECIENT_FUND;

  retVal = updateAccountBalance(&(transData->terminalData), accountLoc);
  if (retVal != SERVER_OK) return INTERNAL_SERVER_ERROR;

  return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t const *const cardData,
                                int *const index) {
  /*
      This function will take card data and validate these data.
      It checks if the PAN exists or not in the server's database.
      If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will
     return OK
  */
  for (uint8_t i = 0; i < Accounts_DB_top; i++) {
    if (strcmp(cardData->primaryAccountNumber,
               Accounts_DB[i].primaryAccountNumber) == 0) {
      *index = i;
      return SERVER_OK;
    }
  }
  return DECLINED_STOLEN_CARD;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t const *const termData,
                                   const int index) {
  /*
      This function will take terminal data and validate these data.
      It checks if the transaction's amount is available or not.
      If the transaction amount is greater than the balance in the database
     will
     return LOW_BALANCE, else will return OK
  */
  if (Accounts_DB[index].balance < termData->transAmount)
    return LOW_BALANCE;
  else
    return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t *const transData) {
  /*
      This function will take all transaction data into the transactions
     database.
      It gives a sequence number to a transaction, this number is incremented
     once a transaction is processed into the server.
      If saves any type of transaction, APPROVED or DECLINED, with the
     specific
     reason for declining/transaction state.
      If transaction can't be saved will return SAVING_FAILED, else will
     return
     OK
  */
  if (Transctions_DB_top == BUFFER_SIZE) return SAVING_FAILED;
  transData->transactionSequenceNumber = Transctions_DB_top + 1;
  Transctions_DB[Transctions_DB_top] = *transData;
  Transctions_DB_top++;
  return SERVER_OK;
}
EN_serverError_t getTransaction(uint32_t const transactionSequenceNumber,
                                ST_transaction_t *const transData) {
  /*
    This function will take a transaction sequence number and search for this
  transaction in the database.
    If the transaction can't be found will return TRANSACTION_NOT_FOUND, else
  will return OK and store the transaction in a structure
  */
  ST_transaction_t *Trx = &(Transctions_DB[0]);
  for (uint8_t i = 0; i < Transctions_DB_top; i++, Trx++) {
    if (Trx->transactionSequenceNumber == transactionSequenceNumber) {
      *transData = *Trx;
      return SERVER_OK;
    }
  }
  return TRANSACTION_NOT_FOUND;
}
void DB_change(void) {
  for (int i = 0; i < Transctions_DB_top; i++) {
    // correct all the sequence number of the fabricated Trx DB
    Transctions_DB[i].transactionSequenceNumber = i + 1;
  }
}