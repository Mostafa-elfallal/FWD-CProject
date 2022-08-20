#ifndef __CARD_H_
#define __CARD_H_
#define _CRT_SECURE_NO_WARNINGS
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#define MAX_BUFFER_LEN 30
#define MAX_NAME_LEN 25
#define MIN_NAME_LEN 20
#define MAX_NUM_LEN 19
#define MIN_NUM_LEN 16

typedef struct ST_Date_t {
  uint8_t day;
  uint8_t month;
  uint32_t year;
} ST_Date_t;
typedef struct ST_cardData_t {
  uint8_t cardHolderName[MAX_NAME_LEN];
  uint8_t primaryAccountNumber[MAX_NUM_LEN];
  ST_Date_t cardExpirationDate;
  FILE *fptr;
} ST_cardData_t;

typedef enum EN_cardError_t {
  OK,
  WRONG_NAME,
  WRONG_EXP_DATE,
  WRONG_PAN
} EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t *const cardData);

EN_cardError_t getCardExpiryDate(ST_cardData_t *const cardData);

EN_cardError_t getCardPAN(ST_cardData_t *const cardData);

#endif /* __CARD_H_ */