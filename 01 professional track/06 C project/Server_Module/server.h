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
 * |    @brief : this file contains all required typedefs, micros and function prototypes needed for the server side to run             |
 * |             this file would be useful for checking if the account is valid or not and whether the required amount is available     |
 * |             and to update the account information if the operation is successful                                                   | 
 * |    @notes : every function notes, information about its use and what are the parameters used to be passed in are mentioned in      |
 * |             the "server.c" file above each function definition                                                                     |
 * --------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _SERVER_INCLUDE_H_
#define _SERVER_INCLUDE_H_

/*get the necessary includes*/
#include <stdint.h>
#include "../Card_Module/card.h"
#include "../Terminal_Module/terminal.h"

/*************************************************** maacros & structs & typedefs ****************************************************/

// database related data
#define ACCOUNT_DB_NAME                 "./Server_Module/accounts_database.txt"
#define TRANS_DB_NAME                   "./Server_Module/transactions_database.txt"
#define MAX_LINE_SIZE_DB                200

//node account related data
#define ACCOUNT_PAN_SIZE                16
#define ACCOUNT_NAME_SIZE               70
#define ACCOUNT_STATE_SIZE              7

//node transaction related data
#define TRANS_STATE_SIZE                30
#define TRANS_DATE_SIZE                 10

//account status
#define ACC_STATE_RUN                   "running"
#define ACC_STATE_BLOCK                 "blocked"

//transactions status
#define TRANS_STATE_COMP                "completed"
#define TRANS_STATE_PEND                "pending"
#define TRNAS_STATE_REF                 "refused"
#undef  MAX_FLOAT_CHAR_LEN
#define MAX_FLOAT_CHAR_LEN              25
#define MAX_FLOAT_PREC_STORE            "%.6f"


/*enum used to get the transaction error status*/
typedef enum EN_transState_t
{
    PENDING                     = 0, 
    COMPLETED                   = 1, 
    REFUSED                     = 2, 
}EN_transState_t;
 
/*enum used to get the server error status*/
typedef enum EN_serverError_t
{
    SERVER_OK               = 0, 
    SAVING_FAILED           = 1, 
    TRANSACTION_NOT_FOUND   = 2,
    ACCOUNT_NOT_FOUND       = 3, 
    LOW_BALANCE             = 4,   
    BLOCKED_ACCOUNT         = 5,
}EN_serverError_t ;

/*enum used to get the account state whether it's blocked or running*/
typedef enum EN_accountState_t
{
    RUNNING                 = 0,
    BLOCKED                 = 1,
}EN_accountState_t;

/*account node*/
typedef struct ST_accountsDB_t
{
    float balance;
    struct ST_accountsDB_t *next;
    ST_cardData_t cardHolderData;
    EN_accountState_t state;
}ST_accountsDB_t;

/*transaction node*/
typedef struct ST_transaction_t
{
    EN_transState_t transState;
    struct ST_transaction_t *next;
    ST_cardData_t cardHolderDataFrom;
    ST_cardData_t cardHolderDataTo;
    ST_terminalData_t terminalData;
}ST_transaction;

/*list of all accounts*/
typedef struct{
    ST_accountsDB_t* head;
    ST_accountsDB_t* tail;
    int32_t size;
}accounts_list;

/*list of all transactions*/
typedef struct{
    ST_transaction* head;
    ST_transaction* tail;
    int32_t size;
}transactions_list;



/*
 *  @note: the last 4 numbers of the PAN number are generated due to an equation made up of my own own which is 
 *  ((12 MSB number * 5) %10) * 1000 + ((12 MSB number / 2) %10) * 100 + ((((12 MSB number + 3) * 2) / 5) %10) * 10 + (12 MSB number + 2) %10
 */


/*************************************************** function prototypes ****************************************************/

/*
 *@public_functions
 */
void serverInit(void);                                                                                      /*<must be called before using any other function in the server>*/

EN_serverError_t recieveTransactionData(ST_cardData_t *arg_RcvCard_t);                                      /*function that called regularly to iterate over all pending transactions and update amounts*/
EN_serverError_t isValidAccount(ST_cardData_t cardData);                                                    /*function that checks is someone exist there in the database or not*/
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_cardData_t *arg_SenderCard_t);           /*function that checks that if someone has the enough amount in their balance to complete payment*/
EN_serverError_t saveTransaction(ST_transaction *transData);                                                /*function that saves the transaction in both database and in program*/
EN_serverError_t isBlockedAccount(ST_cardData_t *args_Card_t);                                              /*fucntion to check whether that account is blocked or not*/
EN_serverError_t isThatPANThere(ST_cardData_t* arg_Card_t);                                                 /*a function to check if someone that will recive the money is there in that database*/
EN_serverError_t blockAccount(ST_cardData_t* arg_Card_t);                                                   /*a function to block the user card*/


void showCurrentBalance(ST_cardData_t* arg_Card_t);                                                         /*a function to show th current balance of this account*/
void showAllPrvTrans(ST_cardData_t* arg_Card_t);                                                            /*a function to show all previous trans related to this account*/








#endif