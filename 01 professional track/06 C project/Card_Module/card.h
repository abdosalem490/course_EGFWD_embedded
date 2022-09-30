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
 * |    @brief : this file contains all required typedefs, micros and function prototypes needed for the card side to run               |
 * |             this file would be useful for getting some primary information like PAN , card name holder , etc...                    |
 * |    @notes : every function notes, information about its use and what are the parameters used to be passed in are mentioned in      |
 * |             the "card.c" file above each function definition                                                                       |
 * --------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _CARD_INCLUDE_H_
#define _CARD_INCLUDE_H_

#include <stdint.h>

/*************************************************** maacros & structs & typedefs ****************************************************/
#ifndef ACCOUNT_NAME_SIZE
    #define ACCOUNT_NAME_SIZE           70
#endif

#ifndef ACCOUNT_MIN_NAME_LEN
    #define ACCOUNT_MIN_NAME_LEN        4
#endif

#ifndef ACCOUNT_EXPIRY_DATE_LEN
    #define ACCOUNT_EXPIRY_DATE_LEN     5
    #define ACCOUNT_EXPIRY_MON_LEN      2
    #define ACCOUNT_EXPIRY_YEAR_LEN     2
#endif

#ifndef ACCOUNT_PAN_SIZE
    #define ACCOUNT_PAN_SIZE            16
#endif

/*handle structure to handle all the data related to the card owner*/
typedef struct ST_cardData_t
{
    uint8_t cardHolderName[ACCOUNT_NAME_SIZE];
    uint8_t primaryAccountNumber[ACCOUNT_PAN_SIZE + 1];
    uint8_t cardExpirationDate[ACCOUNT_EXPIRY_DATE_LEN + 1];
}ST_cardData_t;

/*psossible errors that could occur*/
typedef enum EN_cardError_t
{
    CARD_OK = 0,
    WRONG_NAME = 1,
    WRONG_EXP_DATE = 2,
    WRONG_PAN = 3,
}EN_cardError_t;

/*************************************************** global function prototypes ****************************************************/
EN_cardError_t getCardHolderName(ST_cardData_t *cardData);                       /*<get the name of the person who holds that credit card and return that name in form of string>*/                   
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);                       /*<get the expiry date of that credit card and return that date in form of string in form of month/year>*/  
EN_cardError_t getCardPAN(ST_cardData_t *cardData);                              /*<get the primary account number (16 numbers) of the transmitter from the user and return it in form of string*/  

#endif