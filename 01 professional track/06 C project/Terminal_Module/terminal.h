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
 * |    @brief : this file contains all required typedefs, micros and function prototypes needed for the terminal side to run           |
 * |             this file would be useful for getting all card data and get transaction date and check whether the card is expored     |
 * |             and accept or decline the amount of money to be transacted                                                             | 
 * |    @notes : every function notes, information about its use and what are the parameters used to be passed in are mentioned in      |
 * |             the "terminal.c" file above each function definition                                                                   |
 * --------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _TERMINAL_INCLUDE_H_
#define _TERMINAL_INCLUDE_H_

#include <stdint.h>

/*************************************************** maacros & structs & typedefs ****************************************************/
#define MAX_AMOUNT_DB_NAME                  "./Terminal_Module/maxAmount_db.txt"

#undef MAX_LINE_SIZE_DB
#define MAX_LINE_SIZE_DB                    15

#define ACCOUNT_TRANS_DATE_LEN              11

#define MAX_PASS_LEN                        15


/*************************************************** global function prototypes ****************************************************/

void terminalInit(void);                                                    /*has to be called first before using any other function and called only once in the main program*/
int8_t* getTransactionDate(void);                                           /*get the date of commiting the transaction from the system date*/
_Bool isCardExpried(int8_t* arg_arrs8ExpiryDate[]);                                                  /*compares the current date from the system with the date on the card to see if it's valid*/
uint32_t gatTransactionAmount(void);                                        /*get the amount of the money the user wants to transact*/
_Bool isBelowMaxAmount(void);                                               /*check if the amount to be transacted is less than the max allowed limit*/
void setMaxAmount(uint32_t arg_u32MaxAmount);                               /*set the new max allowed aount to be transacted*/

/*************************************************** private function prototypes ****************************************************/
static int8_t* getCurrentDate(void);                                        /*get the current date from the device system*/

#endif