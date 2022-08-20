#ifndef __TERMINAL_H_
#define __TERMINAL_H_

#include "../Card/card.h"

typedef struct ST_terminalData_t {
  float transAmount;
  float maxTransAmount;
  ST_Date_t transactionDate;
} ST_terminalData_t;

typedef enum EN_terminalError_t {
  TER_OK,
  WRONG_DATE,
  EXPIRED_CARD,
  INVALID_CARD,
  INVALID_AMOUNT,
  EXCEED_MAX_AMOUNT,
  INVALID_MAX_AMOUNT
} EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t *const termData);

EN_terminalError_t isCardExpired(ST_cardData_t const *const cardData,
                                 ST_terminalData_t const *const termData);

EN_terminalError_t isValidCardPAN(ST_cardData_t const *const cardData);

EN_terminalError_t getTransactionAmount(ST_terminalData_t *const termData);

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t const *const termData);

EN_terminalError_t setMaxAmount(ST_terminalData_t *const termData);

#endif /* __TERMINAL_H_ */