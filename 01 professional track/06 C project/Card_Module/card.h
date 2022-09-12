/*
 * --------------------------------------------------------------------------------------------------------------------------------------
 * |   @author                  : abdo salm (abdelrahman mohamed salem)                                                                 |
 * |   @date_of_creation        : 06/09/2022                                                                                            |
 * |   @version                 : 1.0                                                                                                   |
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
    #define ACCOUNT_EXPIRY_DATE_LEN     8
    #define ACCOUNT_EXPIRY_MON_LEN      2
    #define ACCOUNT_EXPIRY_YEAR_LEN     4
#endif

#ifndef ACCOUNT_PAN_SIZE
    #define ACCOUNT_PAN_SIZE            16
#endif

/*************************************************** global function prototypes ****************************************************/
int8_t* getCardHolderName(void);                        /*<get the name of the person who holds that credit card and return that name in form of string>*/                   
int8_t* getCardExpiryDate(void);                        /*<get the expiry date of that credit card and return that date in form of string in form of month/year>*/  
int8_t* getCardPAN(void);                               /*<get the primary account number (16 numbers) of the transmitter from the user and return it in form of string*/  
int8_t* getRecieverAccountPAN(void);                    /*<get the primary account number (16 numbers) of the reciever from the user and return it in form of string>*/

#endif