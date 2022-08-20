#ifndef __SERVER_H_
#define __SERVER_H_
#include "../Terminal/terminal.h"

typedef enum EN_transState_t {
  APPROVED,
  DECLINED_INSUFFECIENT_FUND,
  DECLINED_STOLEN_CARD,
  INTERNAL_SERVER_ERROR
} EN_transState_t;

typedef struct ST_transaction_t {
  ST_cardData_t cardHolderData;
  ST_terminalData_t terminalData;
  EN_transState_t transState;
  uint32_t transactionSequenceNumber;
} ST_transaction_t;

typedef enum EN_serverError_t {
  SERVER_OK,
  SAVING_FAILED,
  TRANSACTION_NOT_FOUND,
  ACCOUNT_NOT_FOUND,
  LOW_BALANCE
} EN_serverError_t;

typedef struct ST_accountsDB_t {
  float balance;
  uint8_t primaryAccountNumber[MAX_NUM_LEN];
} ST_accountsDB_t;

EN_transState_t recieveTransactionData(ST_transaction_t *const transData);

EN_serverError_t isValidAccount(ST_cardData_t const *const cardData,
                                int *const index);

EN_serverError_t isAmountAvailable(ST_terminalData_t const *const termData,
                                   const int index);

EN_serverError_t saveTransaction(ST_transaction_t *const transData);

EN_serverError_t getTransaction(uint32_t const transactionSequenceNumber,
                                ST_transaction_t *const transData);

void DB_load(void);
void DB_store(void);
void DB_change(void);
#endif /* __SERVER_H_ */