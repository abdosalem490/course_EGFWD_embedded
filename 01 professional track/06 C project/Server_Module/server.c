#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
 *@private_functions
 */
static void updateAccountDB();                                                                          /*<private func used to update a single row in database of accounts>*/
static void updateTransactionDB();                                                                      /*<private func used to update a single row in database of transactions>*/                       
static void insertIntoAccountsListFromDatabase(int8_t *value);                                          /*<private func used to add a new node to linked list of accounts>*/
static void insertIntoTransListFromDatabase(int8_t *value);                                             /*<private func used to add a new node to linked list of transactions>*/
static int8_t* getSenderName(int8_t* arg_senderPAN_t);                                                  /*<private function used to get the name of the person who sended the money>*/

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

/*  @fn         :       -recieveTransactionData
 *
 *  @params[0]  :       -the PAN number of the account that will receive the transaction
 *
 *  @brief      :       -loops over all accounts and update the new balance if there is a transaction
 *
 *  @notes      :       -must be called after signing in immediatly and make sure that the PAN already exist in the database
 */
void recieveTransactionData(int8_t* arg_arrs8ReceiverPAN)
{
    /*create a dummy pointer and use it to loop all over the transactions*/
    account_transaction_node *local_trans_t = transactionsHead.head;

    /*create a dummy pointer and use it to loop all over the accounts*/   
    account_data_node *local_acc_t = accountsHead.head;

    /*flag to indicate if there is inComming transactions*/
    _Bool local_boolIsThereTrans = 0;

    /*dummy variable to see if the receiver wants to accept the transaction or not*/ 
    int8_t *local_s8IsTransactionAccept = 0;

    /*loop all over accounts till get the correct account to update*/
    while(strncmp((const char*)arg_arrs8ReceiverPAN, (const char*)local_acc_t->PAN, strlen((char*)arg_arrs8ReceiverPAN)) != 0)
    {
        local_acc_t = local_acc_t->next;
    }

    /*get all incoming transactions*/
    while(local_trans_t != NULL)
    {
        /*check if this transaction is coming to me*/
        if(strncmp((const char*)local_trans_t->PAN_to, (const char*)arg_arrs8ReceiverPAN, ACCOUNT_PAN_SIZE) == 0)
        {
            /*get the name of the person who is sending this money*/
            int8_t* local_arrs8SenderName = getSenderName(local_trans_t->PAN_from);

            /*tell the user that there is someone who is seding them a money and ask then for apporval*/
            printf("a person with name %s wants to send you %f\n", (char*)local_arrs8SenderName, local_trans_t->amount);
            printf("click [y] to accept , click [n] to refuse:\n");

            /*get the input from the user*/
            scanf("%c", &local_s8IsTransactionAccept);

            /*check if the user entered 'y'*/
            if(local_s8IsTransactionAccept == 'y')
            {
                /*make the flag 1 to be used in order to update the database*/
                local_boolIsThereTrans = 1;

                /*mark the state of the transactions to be completed*/
                

                /*update the account balance with the new balance*/


            }
            else if(local_s8IsTransactionAccept == 'n')
            {
                /*make the state of the transaction as refused*/

                /*return the sender their money back*/
                
            }

        }

        /*check the next transaction*/
        local_trans_t = local_trans_t->next; 
    }

    /*print a message to tell the user there is no incoming transactions today*/
    if(local_boolIsThereTrans == 0)
    {
        fputs("there is no incoming transactions, check up later\n", stdout);
    }
    /*update the database if there any change in the linked lists*/
    else
    {
        updateTransactionDB();  //update the transactions database
        updateAccountDB();      //update the accounts database
    }

}

/*  @fn         :       -getSenderName
 *
 *  @params[0]  :       -the PAN number of the person who is sedning the money
 *
 *  @brief      :       -gets the name of the person who sent the money from the linked list
 *
 *  @notes      :       -none
 */
static int8_t* getSenderName(int8_t* arg_senderPAN_t)
{
    /*create a dummy pointer and use it to loop all over the accounts*/   
    account_data_node *local_acc_t = accountsHead.head;

    /*checks if the PANs match with each other then we found the number*/
    while(strncmp((const char*)local_acc_t->PAN, (const char*)arg_senderPAN_t, ACCOUNT_PAN_SIZE) != 0)
    {
        local_acc_t = local_acc_t->next;
    }

    return local_acc_t->name;
}

/*  @fn         :       -isValidAccount
 *
 *  @params[0]  :       -a structure of type account_data_node to test whether this account exists or not
 *
 *  @brief      :       -loops over all accounts and check whether if there is account with same PAN , holder name and expiry date
 *
 *  @notes      :       -should be called before making any transactions
 *
 *  @return     :       - 1 : if the account is valid
 *                        0 : if the account is invalid
 */
_Bool isValidAccount(account_data_node *arg_account_t)
{
    /*local boolean flags to indicate whether the account is valid or not*/
    _Bool local_boolIsValidPAN  = 0;     // checks for the PAN
    _Bool local_boolIsValidNAME = 0;     // checks for the car holder name
    _Bool local_boolIsValidEXP  = 0;     // checks for the expiry date

    /*dummy pointer used to iterate over the linked list of accounts*/
    account_data_node *local_ptrDummy = accountsHead.head;

    /*linear seaching on the linked list for that account using its PAN*/
    while(local_ptrDummy != NULL)
    {   
        /*check if the 2 PANs are same*/
        if(strncmp((const char*)arg_account_t->PAN, (const char*)local_ptrDummy->PAN, ACCOUNT_PAN_SIZE) == 0)
        {
            local_boolIsValidPAN = 1;
            break;
        }   

        /*get to next element*/
        local_ptrDummy = local_ptrDummy->next;
    }

    /*checks if we found the pan or not*/
    if(local_boolIsValidPAN == 1)
    {
        /*checks for the holder name*/
        if(strncmp((const char*)arg_account_t->name, (const char*)local_ptrDummy->name, ACCOUNT_NAME_SIZE) == 0)
        {
            local_boolIsValidNAME = 1;
        }  
        else
        {
            fputs("inValid holder name\n", stdout);
        }
     
        /*checks for the expiry date*/
        if(arg_account_t->year == local_ptrDummy->year && arg_account_t->month == local_ptrDummy->month)
        {
            local_boolIsValidEXP = 1;
        }  
        else
        {
            fputs("inValid expiry date \n", stdout);
        }

    }
    else
    {
        fputs("the entered PAN is invalid\n" ,stdout);
    }

    return (local_boolIsValidPAN & local_boolIsValidNAME & local_boolIsValidEXP);
}

/*  @fn         :       -isAmountAvailable
 *
 *  @params[0]  :       -a structure of type account_data_node to search for this PAN number in the database
*
 *  @params[1]  :       -the amount the is supposed to be transacted
 *
 *  @brief      :       -loops over all accounts and check if the account with same PAN has enought amount or not
 *
 *  @notes      :       -should be called after checking if account isValid
 *
 *  @return     :       - 1 : if the amount is available
 *                        0 : if the amount isn't enough
 */
_Bool isAmountAvailable(account_data_node *arg_account_t, uint32_t arg_u32amount)
{


    return 0;
}

/*  @fn         :       -saveTransaction
 *
 *  @params[0]  :       -a structure of type account_transaction_node to be saved in the database
 *
 *  @brief      :       -saves only one transaction in the database
 *
 *  @notes      :       -should be called after transaferring the amount
 *
 *  @return     :       - none
 */
void saveTransaction(account_transaction_node arg_trans_t)
{

}



/*  @fn     :       -updateTransactionDB
 *
 *  @brief  :       -just updates the whole transaction database
 *
 *  @notes  :       -must be called after each update in transaction linked list
 */
static void updateTransactionDB()
{

}

/*  @fn     :       -updateRecordDB
 *
 *  @brief  :       -just updates the whole account database , especially for amount
 *
 *  @notes  :       -should be called after each update in the account linked list
 */
static void updateAccountDB()
{

}