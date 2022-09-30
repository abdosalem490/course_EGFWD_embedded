/*
 * --------------------------------------------------------------------------------------------------------------------------------------
 * |   @author                  : abdo salm (abdelrahman mohamed salem)                                                                 |
 * |   @date_of_creation        : 21/09/2022                                                                                            |
 * |   @version                 : 2.0                                                                                                   |
 * |   @contact_email           : abdosalm555@gmail.com                                                                                 |
 * |   @Github_account          : https://github.com/abdosalem490                                                                       |
 * |   @stack_overflow_account  : https://stackoverflow.com/users/16305340/abdo-salm                                                    |
 * |   @phone_number            : 01122786504                                                                                           |
 * --------------------------------------------------------------------------------------------------------------------------------------
 * |    @brief : this file contains all required typedefs, micros and function prototypes needed for the terminal side to run           |
 * |             this file would be useful for getting all card data and get transaction date and check whether the card is expored     |
 * |             and accept or decline the amount of money to be transacted                                                             | 
 * |    @notes : every function notes, information about its use and what are the parameters used to be passed in are mentioned in      |
 * |             the "terminal.c" file above each function definition                                                                   |
 * --------------------------------------------------------------------------------------------------------------------------------------
 */

/*get the necessary includes*/
#include <stdint.h>
#include "../Card_Module/card.h"

#ifndef _TERMINAL_INCLUDE_H_
#define _TERMINAL_INCLUDE_H_


/*************************************************** maacros & structs & typedefs ****************************************************/
#define MAX_AMOUNT_DB_NAME                  "./Terminal_Module/maxAmount_db.txt"

#undef MAX_TRANS_LINE_SIZE_DB
    #define MAX_TRANS_LINE_SIZE_DB                    20

#define ACCOUNT_TRANS_DATE_LEN                  10

#define MAX_PASS_LEN                            20

#undef ACCOUNT_EXPIRY_DATE_LEN
    #define ACCOUNT_EXPIRY_DATE_LEN             5

#ifndef MAX_FLOAT_CHAR_LEN
    #define MAX_FLOAT_CHAR_LEN                  25
    #define MAX_FLOAT_PREC_STORE                "%.6f"
#endif

#define CARD_EXPIRY_DATE_LEN                

/*strucuture to handle the transaction*/
typedef struct ST_terminalData_t
{
    float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[ACCOUNT_TRANS_DATE_LEN + 1];
}ST_terminalData_t;

/*enum to define the error type of the function when getting the data*/
typedef enum EN_terminalError_t
{
    TERMINAL_OK         = 0,
    WRONG_DATE          = 1,
    EXPIRED_CARD        = 2,
    INVALID_CARD        = 3, 
    INVALID_AMOUNT      = 4, 
    EXCEED_MAX_AMOUNT   = 5, 
    INVALID_MAX_AMOUNT  = 6,
}EN_terminalError_t ;


/*************************************************** global function prototypes ****************************************************/

void terminalInit(void);                                                                            /*has to be called first before using any other function and called only once in the main program*/
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);                                 /*get the date of commiting the transaction from the system date*/
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);               /*compares the current date from the system with the date on the card to see if it's valid*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);                               /*get the amount of the money the user wants to transact*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);                                   /*check if the amount to be transacted is less than the max allowed limit*/
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData);                                       /*set the new max allowed aount to be transacted*/  
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);                                         /*used to make sure that the entered PAN isn't fake*/                                                            

int8_t* getCurrentDate(void);                                                                       /*get the current date from the device system*/

#endif