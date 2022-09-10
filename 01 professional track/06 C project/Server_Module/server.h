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
 * |    @brief : this file contains all required typedefs, micros and function prototypes needed for the server side to run             |
 * |             this file would be useful for checking if the account is valid or not and whether the required amount is available     |
 * |             and to update the account information if the operation is successful                                                   | 
 * |    @notes : every function notes, information about its use and what are the parameters used to be passed in are mentioned in      |
 * |             the "server.c" file above each function definition                                                                     |
 * --------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _SERVER_INCLUDE_H_
#define _SERVER_INCLUDE_H_

#include <stdint.h>

/*************************************************** maacros & structs & typedefs ****************************************************/

// database related data
#define ACCOUNT_DB_NAME                 "./Server_Module/accounts_database.txt"
#define TRANS_DB_NAME                   "./Server_Module/transactions_database.txt"
#define MAX_LINE_SIZE_DB                200

//node account related data
#define ACCOUNT_PAN_SIZE                16
#define ACCOUNT_NAME_SIZE               70

//node transaction related data
#define TRANS_STATE_SIZE                30
#define TRANS_DATE_SIZE                 10

typedef struct account_node{
    float balance;
    uint16_t year;
    uint8_t month;
    struct account_node *next;
    int8_t name[ACCOUNT_NAME_SIZE+1];
    int8_t PAN[ACCOUNT_PAN_SIZE+1];
}account_data_node;

typedef struct transaction_node{
    float amount;
    int8_t date[TRANS_DATE_SIZE+1];
    int8_t PAN_from[ACCOUNT_PAN_SIZE+1];
    int8_t PAN_to[ACCOUNT_PAN_SIZE+1];
    int8_t state[TRANS_STATE_SIZE];
    struct transaction_node *next;
}account_transaction_node;


typedef struct{
    account_data_node* head;
    account_data_node* tail;
    int32_t size;
}accounts_list;

typedef struct{
    account_transaction_node* head;
    account_transaction_node* tail;
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
void serverInit(void);                                                                                  /*<must be called before using any other function in the server>*/

void recieveTransactionData(int8_t* arg_arrs8ReceiverPAN);                                              /*function that called regularly to iterate over all pending transactions and update amounts*/
_Bool isValidAccount(account_data_node *arg_account_t);                                                 /*function that checks is someone exist there in the database or not*/
_Bool isAmountAvailable(account_data_node *arg_account_t, uint32_t arg_u32amount);                      /*function that checks that if someone has the enough amount in their balance to complete payment*/
void saveTransaction(account_transaction_node arg_trans_t);                                             /*function that saves the transaction in both database and in program*/



#endif