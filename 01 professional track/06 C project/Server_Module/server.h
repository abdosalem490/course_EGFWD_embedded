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
void serverInit(void);                              /*<must be called before using any other function in the server>*/

void recieveTransactionData();
void isValidAccount();
void isAmountAvailable();
void saveTransaction();

/*
 *@private_functions
 */
static void updateRecordAccount(uint16_t arg_u16RowNum, accounts_list *arg_account_t);                  /*<private func used to update a single row in database of accounts>*/
static void addNewTransactionRecord(account_transaction_node *arg_trans_t);                             /*<private func used to update a single row in database of transactions>*/                       
static void insertIntoAccountsListFromDatabase(int8_t *value);                                          /*<private func used to add a new node to linked list of accounts>*/
static void insertIntoTransListFromDatabase(int8_t *value);                                             /*<private func used to add a new node to linked list of transactions>*/


#endif