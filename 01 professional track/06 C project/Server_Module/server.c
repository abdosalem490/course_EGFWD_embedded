#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*************************************************** global variables ****************************************************/
static accounts_list accountsHead = {NULL, NULL, 0};
static transactions_list transactionsHead = {NULL, NULL, 0};

//the structure of the accounts_database file is as follows where \n and ~ are used as delimeters
/*
<account_number_1_PAN>~<account_number_1_expire_data>~<account_number_1_holder_name>~<account_number_1_balance_amount>
.......
.......
.......
.......
*/

//the structure of the transactions_database file is as follows where \n\r and ~ are used as delimeters
/*
<account_number_1_PAN_from>~<account_number_1_PAN_to>~<amount>~<date>~<state>
.......
.......
.......
.......
*/



/*  @fn     :       -serverInit
 *
 *  @params :       -none
 *
 *  @notes  :       -this function must get called to before interacting with the server functionalities  
 *
 *  @brief  :       -this function do all required initializations like reading data from database
 */
void serverInit(void)
{
    /*temp variable used to read from the database line by line*/
    int8_t local_s8ArrDummy[MAX_LINE_SIZE_DB];
    
    /*pointer that points to the file*/
    FILE *file;

    /*open database account file for read only*/
    file = fopen(ACCOUNT_DB_NAME, "r");

    /*reading from account database file line by line*/
    while(fgets((char*)local_s8ArrDummy, MAX_LINE_SIZE_DB, file) != NULL)
    {
        /*insert into the linked list of accounts*/
        insertIntoAccountsListFromDatabase(local_s8ArrDummy);
    }

    /*close database account file*/
    fclose(file);

    /*open database transaction file for read only*/
    file = fopen(TRANS_DB_NAME, "r");

    /*reading from transaction database file line by line*/
    while(fgets((char*)local_s8ArrDummy, MAX_LINE_SIZE_DB, file) != NULL)
    {
        /*insert into the linked list of accounts*/
        insertIntoTransListFromDatabase(local_s8ArrDummy);
    }

    /*close database account file*/
    fclose(file);

/****************************************************/
    /*testing purposes*/
    /*account_data_node *t1 = accountsHead.head;
    while(t1)
    {
        printf("%s\t%d/%d\t%s\t%f\n",t1->PAN, t1->month, t1->year, t1->name, t1->balance);
        t1 = t1->next;
    }
    printf("size of transactions = %d\n", accountsHead.size);
    
    account_transaction_node *t2 = transactionsHead.head;
    while(t2)
    {
        printf("%s\t%s\t%f\t%s\t%s\n",t2->PAN_from, t2->PAN_to, t2->amount, t2->date, t2->state);
        t2 = t2->next;
    }
    printf("size of transactions = %d\n", transactionsHead.size);*/
/****************************************************/

}

/*  @fn     :       -insertIntoAccountsListFromDatabase
 *
 *  @params :       -string values with delimeter '~' to separate between fields
 *
 *  @brief  :       -this function insert all nodes into the linked list
 */
static void insertIntoAccountsListFromDatabase(int8_t *value)
{
    /*dummy variable used to iterated over the data in one node*/
    int8_t* local_s8Dummy = value + ACCOUNT_PAN_SIZE + 1;

    /*create a new node*/
    account_data_node *local_newNode_t = (account_data_node*) malloc(sizeof(account_data_node));

    /*initialze all data with zeros*/
    memset(local_newNode_t, 0, sizeof(account_data_node));
    local_newNode_t->next = NULL;

    /*get the card PAN*/
    strncpy((char*)local_newNode_t->PAN, (const char*)value, ACCOUNT_PAN_SIZE);
    local_newNode_t->PAN[ACCOUNT_PAN_SIZE] = '\0';
    
    /*get the date of expiry where the format is month/year */
    local_newNode_t->month = 10 * (int)(*local_s8Dummy - '0');    local_s8Dummy++;    local_newNode_t->month += (int)(*local_s8Dummy - '0'); local_s8Dummy++; local_s8Dummy++;
    local_newNode_t->year = 1000 * (int)(*local_s8Dummy - '0');   local_s8Dummy++;    local_newNode_t->year += 100 * (int)(*local_s8Dummy - '0');   local_s8Dummy++;    local_newNode_t->year += 10 * (int)(*local_s8Dummy - '0');   local_s8Dummy++;    local_newNode_t->year += (int)(*local_s8Dummy - '0');   local_s8Dummy++; local_s8Dummy++;

    /*get the owner name*/
    uint8_t i = 0;
    for(; *local_s8Dummy != '~'; i++)
    {
        local_newNode_t->name[i] = *local_s8Dummy;
        local_s8Dummy++;
    }
    local_newNode_t->name[i] = '\0';
    local_s8Dummy++;
    
    

    /*get the balance*/
    while(*local_s8Dummy!= '~')
    {
        local_newNode_t->balance *= 10;
        local_newNode_t->balance += (float)(*local_s8Dummy - '0');
        local_s8Dummy++;
    }
    

    /*insert the node into the list*/
    if(accountsHead.head == NULL)
    {
        accountsHead.head = local_newNode_t;
        accountsHead.tail = local_newNode_t;
    }
    else
    {
        accountsHead.tail->next = local_newNode_t;
        accountsHead.tail = local_newNode_t;
    }

    /*increment number of elements present in the linked list*/
    accountsHead.size++;
}


/*  @fn     :       -insertIntoTransListFromDatabase
 *
 *  @params :       -string values with delimeter '~' to separate between fields
 *
 *  @brief  :       -this function insert all nodes into the linked list
 */
static void insertIntoTransListFromDatabase(int8_t *value)
{
    /*dummy variable used to iterated over the data in one node*/
    int8_t* local_s8Dummy = value + 2 * (ACCOUNT_PAN_SIZE + 1);

    /*create a new node*/
    account_transaction_node *local_newNode_t = (account_transaction_node*) malloc(sizeof(account_transaction_node));

    /*initialze all data with zeros*/
    memset(local_newNode_t, 0, sizeof(account_transaction_node));
    local_newNode_t->next = NULL;

    /*get the card PAN from*/
    strncpy((char*)local_newNode_t->PAN_from, (const char*)value, ACCOUNT_PAN_SIZE);
    local_newNode_t->PAN_from[ACCOUNT_PAN_SIZE] = '\0';

    /*get the card PAN to*/
    strncpy((char*)local_newNode_t->PAN_to, (const char*)(value + ACCOUNT_PAN_SIZE + 1), ACCOUNT_PAN_SIZE);
    local_newNode_t->PAN_to[ACCOUNT_PAN_SIZE] = '\0';
   
    /*get the amount*/
    while(*local_s8Dummy!= '~')
    {
        local_newNode_t->amount *= 10;
        local_newNode_t->amount += (float)(*local_s8Dummy - '0');
        local_s8Dummy++;
    }
    local_s8Dummy++;

    /*get the date of transaction */
    strncpy((char*)local_newNode_t->date, (const char*)local_s8Dummy, TRANS_DATE_SIZE);
    local_newNode_t->date[TRANS_DATE_SIZE] = '\0';
    local_s8Dummy += (TRANS_DATE_SIZE + 1);


    /*get the state of the transaction*/
    uint8_t i = 0;
    for(; *local_s8Dummy != '~'; i++)
    {
        local_newNode_t->state[i] = *local_s8Dummy;
        local_s8Dummy++;
    }
    local_newNode_t->state[i] = '\0';
    local_s8Dummy++;

    /*insert the node into the list*/
    if(transactionsHead.head == NULL)
    {
        transactionsHead.head = local_newNode_t;
        transactionsHead.tail = local_newNode_t;
    }
    else
    {
        transactionsHead.tail->next = local_newNode_t;
        transactionsHead.tail = local_newNode_t;
    }

    /*increment number of elements present in the linked list*/
    transactionsHead.size++;
}

/*  @fn     :       -addNewTransactionRecord
 *
 *  @params :       -a node of type account_transaction_node to be inserted
 *
 *  @brief  :       -just insert a transaction node into the linked list
 */
static void addNewTransactionRecord(account_transaction_node *arg_trans_t)
{

}