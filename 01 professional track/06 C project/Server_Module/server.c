#include "server.h"
#include "../colors.h"

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
static int8_t* getSenderName(uint8_t* arg_senderPAN_t);                                                  /*<private function used to get the name of the person who sended the money>*/

/*************************************************** global variables ****************************************************/
static accounts_list accountsHead = {NULL, NULL, 0};
static transactions_list transactionsHead = {NULL, NULL, 0};


//the structure of the accounts_database file is as follows where \n and ~ are used as delimeters
/*
<account_number_1_PAN>~<account_number_1_expire_data>~<account_number_1_holder_name>~<account_number_1_balance_amount>~<state>~
.......
.......
.......
.......
*/

//the structure of the transactions_database file is as follows where \n\r and ~ are used as delimeters
/*
<account_number_1_PAN_from>~<account_number_1_PAN_to>~<amount>~<date>~<state>~
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
    /*ST_accountsDB_t *t1 = accountsHead.head;
    while(t1)
    {
        printf("%s\t%s\t%s\t%d\t%f\n",t1->cardHolderData.cardHolderName, t1->cardHolderData.primaryAccountNumber, t1->cardHolderData.cardExpirationDate, t1->state, t1->balance);
        t1 = t1->next;
    }
    printf("size of accounts = %d\n", accountsHead.size);
    
    ST_transaction *t2 = transactionsHead.head;
    while(t2)
    {
        printf("%s\t%s\t%f\t%s\t%d\n",t2->cardHolderDataFrom.primaryAccountNumber, t2->cardHolderDataTo.primaryAccountNumber, t2->terminalData.transAmount, t2->terminalData.transactionDate, t2->transState);
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
    /*dummy array that will be converted to a float value later*/
    int8_t local_arrs8DummyArrFloat[MAX_FLOAT_CHAR_LEN];

    /*dummy variable used to iterated over the data in one node*/
    int8_t* local_s8Dummy = value + ACCOUNT_PAN_SIZE + 1 + ACCOUNT_EXPIRY_DATE_LEN + 1;

    /*dummy array to store in it the string state of the account*/
    int8_t local_arrs8DummyArrState[ACCOUNT_STATE_SIZE + 1];

    /*create a new node*/
    ST_accountsDB_t *local_newNode_t = (ST_accountsDB_t*) malloc(sizeof(ST_accountsDB_t));

    /*initialze all data with zeros*/
    memset(local_newNode_t, 0, sizeof(ST_accountsDB_t));
    local_newNode_t->next = NULL;

    /*get the card PAN*/
    strncpy((char*)local_newNode_t->cardHolderData.primaryAccountNumber, (const char*)value, ACCOUNT_PAN_SIZE);
    local_newNode_t->cardHolderData.primaryAccountNumber[ACCOUNT_PAN_SIZE] = '\0';
    
    /*get the date of expiry where the format is MM/YY */
    strncpy((char*)local_newNode_t->cardHolderData.cardExpirationDate, (const char*)(value + ACCOUNT_PAN_SIZE + 1), ACCOUNT_EXPIRY_DATE_LEN);
    local_newNode_t->cardHolderData.cardExpirationDate[ACCOUNT_EXPIRY_DATE_LEN] = '\0';


    /*get the owner name*/
    uint8_t i = 0;
    for(; *local_s8Dummy != '~'; i++)
    {
        local_newNode_t->cardHolderData.cardHolderName[i] = *local_s8Dummy;
        local_s8Dummy++;
    }
    local_newNode_t->cardHolderData.cardHolderName[i] = '\0';
    local_s8Dummy++;
    
    
    /*get the balance into the dummy array*/
    for(i = 0; *local_s8Dummy != '~'; i++)
    {
        local_arrs8DummyArrFloat[i] = *local_s8Dummy;
        local_s8Dummy++;
    }
    local_arrs8DummyArrFloat[i] = '\0';
    local_s8Dummy++;

    /*convert from string to float*/
    local_newNode_t->balance = strtof((char*)local_arrs8DummyArrFloat, NULL);

    /*get the status of the account*/
    for(i = 0; *local_s8Dummy != '~'; i++)
    {
        local_arrs8DummyArrState[i] = *local_s8Dummy;
        local_s8Dummy++;
    }   
    local_arrs8DummyArrState[i] = '\0';
    local_s8Dummy++;

    /*convert that state into the corresponding value in enum*/
    if(strncmp((const char*)local_arrs8DummyArrState, ACC_STATE_RUN, ACCOUNT_STATE_SIZE) == 0)
    {
        local_newNode_t->state = RUNNING;
    }
    else if(strncmp((const char*)local_arrs8DummyArrState, ACC_STATE_BLOCK, ACCOUNT_STATE_SIZE) == 0)
    {
        local_newNode_t->state = BLOCKED;
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
    /*dummy array that will be converted to a float value later*/
    int8_t local_arrs8DummyArrFloat[MAX_FLOAT_CHAR_LEN];

    /*dummy local array to store in it the state of the transaction*/
    int8_t local_arrs8DummyArrState[TRANS_STATE_SIZE];

    /*dummy variable used to iterated over the data in one node*/
    int8_t* local_s8Dummy = value + 2 * (ACCOUNT_PAN_SIZE + 1);

    /*create a new node*/
    ST_transaction *local_newNode_t = (ST_transaction*) malloc(sizeof(ST_transaction));

    /*initialze all data with zeros*/
    memset(local_newNode_t, 0, sizeof(ST_transaction));
    local_newNode_t->next = NULL;

    /*get the card PAN from*/
    strncpy((char*)local_newNode_t->cardHolderDataFrom.primaryAccountNumber, (const char*)value, ACCOUNT_PAN_SIZE);
    local_newNode_t->cardHolderDataFrom.primaryAccountNumber[ACCOUNT_PAN_SIZE] = '\0';

    /*get the card PAN to*/
    strncpy((char*)local_newNode_t->cardHolderDataTo.primaryAccountNumber, (const char*)(value + ACCOUNT_PAN_SIZE + 1), ACCOUNT_PAN_SIZE);
    local_newNode_t->cardHolderDataTo.primaryAccountNumber[ACCOUNT_PAN_SIZE] = '\0';
   
    /*get the amount into the dummy array*/
    uint8_t i = 0;
    for(i = 0; *local_s8Dummy != '~'; i++)
    {
        local_arrs8DummyArrFloat[i] = *local_s8Dummy;
        local_s8Dummy++;
    }
    local_arrs8DummyArrFloat[i] = '\0';
    local_s8Dummy++;

    /*convert from string to float*/
    local_newNode_t->terminalData.transAmount = strtof((char*)local_arrs8DummyArrFloat, NULL);    

    /*get the date of transaction */
    strncpy((char*)local_newNode_t->terminalData.transactionDate, (const char*)local_s8Dummy, TRANS_DATE_SIZE);
    local_newNode_t->terminalData.transactionDate[TRANS_DATE_SIZE] = '\0';
    local_s8Dummy += (TRANS_DATE_SIZE + 1);

    /*get the state of the transaction*/
    for(i = 0; *local_s8Dummy != '~'; i++)
    {
        local_arrs8DummyArrState[i] = *local_s8Dummy;
        local_s8Dummy++;
    }
    local_arrs8DummyArrState[i] = '\0';
    local_s8Dummy++;

    /*convert that state into its corresponding value as enum*/
    if(strcmp((const char*)local_arrs8DummyArrState, TRANS_STATE_COMP) == 0)
    {
        local_newNode_t->transState = COMPLETED;
    }
    else if(strcmp((const char*)local_arrs8DummyArrState, TRANS_STATE_PEND) == 0)
    {
        local_newNode_t->transState = PENDING;
    }
    else if(strcmp((const char*)local_arrs8DummyArrState, TRNAS_STATE_REF) == 0)
    {
        local_newNode_t->transState = REFUSED;
    }

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
 *  @params[0]  :       -structure containing the PAN number of the account that will receive the transaction
 *
 *  @brief      :       -loops over all accounts and update the new balance if there is a transaction
 *
 *  @notes      :       -must be called after signing in immediatly and make sure that the PAN already exist in the database
 * 
 *  @return     :       -return the status of receiving the transaction
 */
EN_serverError_t recieveTransactionData(ST_cardData_t *arg_RcvCard_t)
{
    /*create a dummy pointer and use it to loop all over the transactions*/
    ST_transaction *local_trans_t = transactionsHead.head;

    /*create a dummy pointer and use it to loop all over the accounts to get reciever*/   
    ST_accountsDB_t *local_accRec_t = accountsHead.head;

    /*create a dummy pointer and use it to loop all over the accounts to get sender*/   
    ST_accountsDB_t *local_accSend_t = accountsHead.head;

    /*flag to indicate the error state*/
    EN_serverError_t local_ErrorState_enum = SERVER_OK;

    /*flag to indicate if there is inComming transactions*/
    _Bool local_boolIsThereTrans = 0;

    /*dummy variable to see if the receiver wants to accept the transaction or not*/ 
    int8_t local_s8IsTransactionAccept = 0;

    /*loop all over accounts till get the correct account to get the receiver account*/
    while(strncmp((const char*)arg_RcvCard_t->primaryAccountNumber, (const char*)local_accRec_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) != 0)
    {
        local_accRec_t = local_accRec_t->next;
    }

    /*get all incoming transactions*/
    while(local_trans_t != NULL)
    {
        /*check if this transaction is coming to me and it's pending*/
        if(strncmp((const char*)local_trans_t->cardHolderDataTo.primaryAccountNumber, (const char*)arg_RcvCard_t->primaryAccountNumber, ACCOUNT_PAN_SIZE) == 0 && 
            local_trans_t->transState == PENDING)
        {
            
            /*get the name of the person who is sending this money*/
            int8_t* local_arrs8SenderName = getSenderName(local_trans_t->cardHolderDataFrom.primaryAccountNumber);

            /*tell the user that there is someone who is seding them a money and ask then for apporval*/
            printf(BLUE "a person with name %s wants to send you %f\n", (char*)local_arrs8SenderName, local_trans_t->terminalData.transAmount);
            printf(CYAN "click [y] to accept , click [n] to refuse:\n" YELLOW);

            /*get the input from the user*/
            scanf("%c", &local_s8IsTransactionAccept);

            /*flush the buffer*/
            fflush(stdin);

            /*check if the user entered 'y'*/
            if(local_s8IsTransactionAccept == 'y')
            {
                /*make the flag 1 to be used in order to update the database*/
                local_boolIsThereTrans = 1;

                /*mark the state of the transactions to be completed*/
                local_trans_t->transState = COMPLETED;

                /*update the account balance with the new balance*/
                local_accRec_t->balance += local_trans_t->terminalData.transAmount;

                fputs(GREEN "operation accepted successfully\n", stdout);

            }
            else if(local_s8IsTransactionAccept == 'n')
            {
                /*make the flag 1 to be used in order to update the database*/
                local_boolIsThereTrans = 1;

                /*make the state of the transaction as refused*/
                local_trans_t->transState = REFUSED;

                /*loop all over accounts till get the correct account to get the sender account*/
                while(strncmp((const char*)local_trans_t->cardHolderDataFrom.primaryAccountNumber, (const char*)local_accSend_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) != 0)
                {
                    local_accSend_t = local_accSend_t->next;
                }

                /*return the sender their money back*/
                local_accSend_t->balance +=  local_trans_t->terminalData.transAmount;

                fputs(GREEN "transaction refused successfully\n", stdout);

            }
            else
            {
                /*do nothing , leave it in the pending state*/
                fputs(RED "the transaction will be left pending\n", stdout);
            }

        }

        /*check the next transaction*/
        local_trans_t = local_trans_t->next; 
    }

    /*print a message to tell the user there is no incoming transactions today*/
    if(local_boolIsThereTrans == 0)
    {
        fputs(WHITE "there is no incoming transactions, check up later\n", stdout);
    }
    /*update the database if there any change in the linked lists*/
    else
    {
        updateTransactionDB();  //update the transactions database
        updateAccountDB();      //update the accounts database
    }

    return local_ErrorState_enum;

}

/*  @fn         :       -getSenderName
 *
 *  @params[0]  :       -the PAN number of the person who is sedning the money
 *
 *  @brief      :       -gets the name of the person who sent the money from the linked list
 *
 *  @notes      :       -none
 */
static int8_t* getSenderName(uint8_t* arg_senderPAN_t)
{
    /*create a dummy pointer and use it to loop all over the accounts*/   
    ST_accountsDB_t *local_acc_t = accountsHead.head;

    /*checks if the PANs match with each other then we found the number*/
    while(strncmp((const char*)local_acc_t->cardHolderData.primaryAccountNumber, (const char*)arg_senderPAN_t, ACCOUNT_PAN_SIZE) != 0)
    {
        local_acc_t = local_acc_t->next;
    }

    return (int8_t*)local_acc_t->cardHolderData.cardHolderName;
}

/*  @fn         :       -isValidAccount
 *
 *  @params[0]  :       -a structure of type ST_cardData_t to test whether this account exists or not
 *
 *  @brief      :       -loops over all accounts and check whether if there is account with same PAN , holder name and expiry date
 *
 *  @notes      :       -should be called before making any transactions
 *
 *  @return     :       -an error state indicating whether that accoint exist or not
 */
 EN_serverError_t isValidAccount(ST_cardData_t cardData)
{
    /*error state to indicate that the account exists or not in the database*/
    EN_serverError_t local_ErrorState_eum = SERVER_OK;

    /*local boolean flags to indicate whether the account is valid or not*/
    _Bool local_boolIsValidPAN  = 0;     // checks for the PAN
    _Bool local_boolIsValidNAME = 0;     // checks for the car holder name
    _Bool local_boolIsValidEXP  = 0;     // checks for the expiry date

    /*dummy pointer used to iterate over the linked list of accounts*/
    ST_accountsDB_t *local_ptrDummy = accountsHead.head;

    /*linear seaching on the linked list for that account using its PAN*/
    while(local_ptrDummy != NULL)
    {   
        /*check if the 2 PANs are same*/
        if(strncmp((const char*)cardData.primaryAccountNumber, (const char*)local_ptrDummy->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) == 0)
        {   
            /*this PAN is valid*/
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
        if(strncmp((const char*)cardData.cardHolderName, (const char*)local_ptrDummy->cardHolderData.cardHolderName, ACCOUNT_NAME_SIZE) == 0)
        {
            local_boolIsValidNAME = 1;
        }  
        else
        {
            fputs(RED "inValid holder name\n", stdout);

            /*change the error state to not found*/
            local_ErrorState_eum = ACCOUNT_NOT_FOUND;
        }
     
        /*checks for the expiry date*/
        if(strncmp((const char*)cardData.cardExpirationDate, (const char*)local_ptrDummy->cardHolderData.cardExpirationDate, ACCOUNT_EXPIRY_DATE_LEN) == 0)
        {
            local_boolIsValidEXP = 1;
        }  
        else
        {
            fputs(RED "inValid expiry date \n", stdout);

            /*change the error state to not found*/
            local_ErrorState_eum = ACCOUNT_NOT_FOUND;
        }

    }
    else
    {
        fputs(RED "the entered PAN is invalid\n" ,stdout);

        /*change the error state to not found*/
        local_ErrorState_eum = ACCOUNT_NOT_FOUND;
    }

    /*checks if the account fulfills that name and PAN and expiry date are there to same account*/
    if(local_boolIsValidPAN & local_boolIsValidNAME & local_boolIsValidEXP)
    {
        /*the account exists*/
        local_ErrorState_eum = SERVER_OK;
    }


    return local_ErrorState_eum;
}

/*  @fn         :       -isAmountAvailable
 *
 *  @params[0]  :       -a struct containg the amount to transact to test the availability of the amount
 *
 *  @params[1]  :       -a struct containg the PAN of the sender to test the availability of the amount
 *
 *  @brief      :       -loops over all accounts and check if the account with same PAN has enought amount or not
 *
 *  @notes      :       -should be called after checking if account isValid
 *
 *  @return     :       -error state indicating whether the account has enough money or not
 */
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_cardData_t *arg_SenderCard_t)
{
    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    ST_accountsDB_t *local_acc_t = accountsHead.head;
    
    /*local variable to indicate the state of the amount is enough or not*/
    EN_serverError_t local_erroState_t = SERVER_OK;

    /*loop all over accounts till get the correct account to get the account*/
    while(strncmp((const char*)arg_SenderCard_t->primaryAccountNumber, (const char*)local_acc_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) != 0)
    {
        local_acc_t = local_acc_t->next;
    }

    /*checks whether the amount is available*/
    if(local_acc_t->balance >=  termData->transAmount)
    {
        local_erroState_t = SERVER_OK;  // amount is available
    }
    else
    {
        local_erroState_t = LOW_BALANCE;  // amount isn't available
    }

    return local_erroState_t;
}

/*  @fn         :       -saveTransaction
 *
 *  @params[0]  :       -a structure of type ST_transaction_t to be saved in the database
 *
 *  @brief      :       -saves one transaction in the database abd deducts the amount of the money from the sender
 *
 *  @notes      :       -should be called to transafer the amount , also the main program should make sure to fulfil all its params
 *
 *  @return     :       -error state indicating the success of saving a transaction
 */
EN_serverError_t saveTransaction(ST_transaction *transData)
{
    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    ST_accountsDB_t *local_acc_t = accountsHead.head;    

    /*loop all over accounts till get the correct account to get the account*/
    while(strncmp((const char*)transData->cardHolderDataFrom.primaryAccountNumber, (const char*)local_acc_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) != 0)
    {
        local_acc_t = local_acc_t->next;
    }

    /*subtract the amount to transact*/
    local_acc_t->balance -= transData->terminalData.transAmount;

    /*saves the transaction in the linked list*/
    transactionsHead.tail->next = transData;
    transactionsHead.tail = transData;

    /*update the transaction datebase*/
    updateTransactionDB();

    /*update the accounts database*/
    updateAccountDB();

    /*will always return SERVER_OK*/
    return SERVER_OK;
}



/*  @fn     :       -updateTransactionDB
 *
 *  @brief  :       -just updates the whole transaction database
 *
 *  @notes  :       -must be called after each update in transaction linked list
 */
static void updateTransactionDB()
{
    /*dummy pointer used to iterate over the elements of transaction linked list*/
    ST_transaction *local_trans_t = transactionsHead.head;

    /*temp variable used to format the string to update database line by line*/
    int8_t local_s8ArrDummy[MAX_LINE_SIZE_DB];
    
    /*pointer that points to the file*/
    FILE *file;

    /*open database account file for read only*/
    file = fopen(TRANS_DB_NAME, "w");

    /*variable to keep track of formatting the string*/
    uint8_t local_u8Index = 0;

    /*iterating over linked list to store it in the database*/
    while(local_trans_t != NULL)
    {
        /*format the string to be stored in the format as :*/
        /*<account_number_1_PAN_from>~<account_number_1_PAN_to>~<amount>~<date>~<state>~*/

        /*copying the "from PAN number"*/
        strncpy((char*)local_s8ArrDummy, (const char*)local_trans_t->cardHolderDataFrom.primaryAccountNumber, ACCOUNT_PAN_SIZE);
        local_u8Index = ACCOUNT_PAN_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*copying the "to PAN number"*/
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_trans_t->cardHolderDataTo.primaryAccountNumber, ACCOUNT_PAN_SIZE);
        local_u8Index += ACCOUNT_PAN_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';   

        /*saving the amount*/
        local_u8Index += sprintf((char*)(local_s8ArrDummy + local_u8Index), MAX_FLOAT_PREC_STORE, local_trans_t->terminalData.transAmount);

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*storing the date*/
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_trans_t->terminalData.transactionDate, TRANS_DATE_SIZE);
        local_u8Index += TRANS_DATE_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*store the state of the transaction*/
        if(local_trans_t->transState == PENDING)
        {
            strcpy((char*)(local_s8ArrDummy + local_u8Index), TRANS_STATE_PEND);
            local_u8Index += strlen((char*)TRANS_STATE_PEND);
        }
        else if(local_trans_t->transState == COMPLETED)
        {
            strcpy((char*)(local_s8ArrDummy + local_u8Index), TRANS_STATE_COMP);
            local_u8Index += strlen((char*)TRANS_STATE_COMP);
        }
        else if(local_trans_t->transState == REFUSED)
        {
            strcpy((char*)(local_s8ArrDummy + local_u8Index), TRNAS_STATE_REF);
            local_u8Index += strlen((char*)TRNAS_STATE_REF);
        }

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*adding a null char at the end*/
        local_s8ArrDummy[local_u8Index++] = '\0';

        /*insert that record in the transaction database*/
        fputs((const char*)local_s8ArrDummy, file);

        /*adding a new line char at the end*/
        fputs("\n", file);

        /*get the next transaction element*/
        local_trans_t = local_trans_t->next;
        
    }

    /*close database account file*/
    fclose(file);

}

/*  @fn     :       -updateAccountDB
 *
 *  @brief  :       -just updates the whole account database , especially for amount
 *
 *  @notes  :       -should be called after each update in the account linked list
 */
static void updateAccountDB()
{
    /*dummy pointer used to iterate over the elements of accounts linked list*/
    ST_accountsDB_t *local_acct_t = accountsHead.head;

    /*temp variable used to format the string to update database line by line*/
    int8_t local_s8ArrDummy[MAX_LINE_SIZE_DB];
    
    /*pointer that points to the file*/
    FILE *file;

    /*open database account file for read only*/
    file = fopen(ACCOUNT_DB_NAME, "w");

    /*variable to keep track of formatting the string*/
    uint8_t local_u8Index = 0;

    /*iterating over linked list to store it in the database*/
    while(local_acct_t != NULL)
    {
        /*format the string to be stored in the format as :*/
        /*<PAN>~<expire_data>~<holder_name>~<balance_amount>~<state>~*/

        /*copying the PAN number*/
        strncpy((char*)local_s8ArrDummy, (const char*)local_acct_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE);
        local_u8Index = ACCOUNT_PAN_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*copying the month expiry date*/
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_acct_t->cardHolderData.cardExpirationDate, ACCOUNT_EXPIRY_DATE_LEN);
        local_u8Index += ACCOUNT_EXPIRY_DATE_LEN;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~'; 

        /*copying the name of the holder*/  
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_acct_t->cardHolderData.cardHolderName, strlen((char*)local_acct_t->cardHolderData.cardHolderName));
        local_u8Index += strlen((char*)local_acct_t->cardHolderData.cardHolderName);   

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*copying the balane amount*/
        local_u8Index += sprintf((char*)(local_s8ArrDummy + local_u8Index), MAX_FLOAT_PREC_STORE, local_acct_t->balance);

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*store the state of the account*/
        if(local_acct_t->state == RUNNING)
        {
            strcpy((char*)(local_s8ArrDummy + local_u8Index), ACC_STATE_RUN);
            local_u8Index += strlen((char*)ACC_STATE_RUN);                   
        }
        else if(local_acct_t->state == BLOCKED)
        {
            strcpy((char*)(local_s8ArrDummy + local_u8Index), ACC_STATE_BLOCK);
            local_u8Index += strlen((char*)ACC_STATE_BLOCK);   
        }
        else
        {
            // do nothing
        }

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*adding a null char at the end*/
        local_s8ArrDummy[local_u8Index++] = '\0';

        /*insert that record in the transaction database*/
        fputs((const char*)local_s8ArrDummy, file);

        /*adding a new line char at the end*/
        fputs("\n", file);

        /*get the next account record*/
        local_acct_t = local_acct_t->next;

    }

    /*close database account file*/
    fclose(file);
}

/*  @fn         :       -showCurrentBalance
 *
 *  @params[0]  :       -an struct contating array of size = ACCOUNT_PAN_SIZE which is PAN of the user
 *
 *  @brief      :       -prints the user current balance
 *
 *  @notes      :       -none
 *
 *  @return     :       -none
 */
void showCurrentBalance(ST_cardData_t* arg_Card_t)
{
    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    ST_accountsDB_t *local_accSend_t = accountsHead.head;

    /*loop all over accounts till get the correct account to get the  account*/
    while(strncmp((const char*)arg_Card_t->primaryAccountNumber, (const char*)local_accSend_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) != 0)
    {
        local_accSend_t = local_accSend_t->next;
    }

    printf(YELLOW "the balance in your account = %f\n", local_accSend_t->balance);

}

/*  @fn         :       -showAllPrvTrans
 *
 *  @params[0]  :       -a struct containing an array of size = ACCOUNT_PAN_SIZE which is PAN of the user
 *
 *  @brief      :       -prints all previous trans made by this user
 *
 *  @notes      :       -none
 *
 *  @return     :       -none
 */
void showAllPrvTrans(ST_cardData_t* arg_Card_t)
{
    /*create a dummy pointer and use it to loop all over the transactions*/
    ST_transaction *local_trans_t = transactionsHead.head;

    /*prints table title*/
    printf(MAGENTA "from\t\t to\t\t  amount\t date\t state\n");

    /*loop all over accounts till get the correct account to get the sender account*/
    while(local_trans_t != NULL)
    {
        /*check if the user got involved in this transaction*/
        if((strncmp((const char*)local_trans_t->cardHolderDataFrom.primaryAccountNumber, (const char*)arg_Card_t->primaryAccountNumber, ACCOUNT_PAN_SIZE) == 0) 
            || (strncmp((const char*)local_trans_t->cardHolderDataTo.primaryAccountNumber, (const char*)arg_Card_t->primaryAccountNumber, ACCOUNT_PAN_SIZE) == 0))
        {
            printf(MAGENTA "%s %s %f %s ",local_trans_t->cardHolderDataFrom.primaryAccountNumber, local_trans_t->cardHolderDataTo.primaryAccountNumber, local_trans_t->terminalData.transAmount, local_trans_t->terminalData.transactionDate);

            /*check conditions for transaction status to know which one to print*/
            if(local_trans_t->transState == PENDING)
            {
                printf("%s\n", TRANS_STATE_PEND);
            }
            else if(local_trans_t->transState == COMPLETED)
            {
                printf("%s\n", TRANS_STATE_COMP);
            }
            else if(local_trans_t->transState == REFUSED)
            {
                printf("%s\n", TRNAS_STATE_REF);
            }
            else
            {
                // do nothing
            }
        }

        /*go to the next node*/
        local_trans_t = local_trans_t->next;
    }
}

/*  @fn         :       -isThatPANThere
 *
 *  @params[0]  :       -a struct containing an array of size = ACCOUNT_PAN_SIZE which is PAN of the user
 *
 *  @brief      :       -checks whether that reciever is there in the database or not
 *
 *  @notes      :       -none
 *
 *  @return     :       -error state indicating whether that PAN number exists in the database or not
 */
 EN_serverError_t isThatPANThere(ST_cardData_t* arg_Card_t)
{
    /*a flag to indicate whether the PAN exist or not*/
    EN_serverError_t local_errorState = ACCOUNT_NOT_FOUND;

    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    ST_accountsDB_t *local_acc_t = accountsHead.head;

    /*loop all over accounts till get the correct account to get the  account*/
    while(local_acc_t != NULL)
    {
        /*check if we found the account*/
        if(strncmp((const char*)arg_Card_t->primaryAccountNumber, (const char*)local_acc_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) == 0)
        {
            /*we found it , YAY*/
            local_errorState = SERVER_OK;
            break;
        }
        /*get the next element in the linked list*/
        local_acc_t = local_acc_t->next;
    }
        
    return local_errorState;
}

/*  @fn         :       -isBlockedAccount
 *
 *  @params[0]  :       -a struct to test whether that account is blocked or not
 *
 *  @brief      :       -checks whether that reciever is there in the database or not
 *
 *  @notes      :       -none
 *
 *  @return     :       -error state indicating whether that account is running or not
 */
EN_serverError_t isBlockedAccount(ST_cardData_t *args_Card_t)
{
    /*local error state variable*/
    EN_serverError_t local_errorSate_t = SERVER_OK;

    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    ST_accountsDB_t *local_acc_t = accountsHead.head;
    
    /*loop all over accounts till get the correct account to get the  account*/
    while(local_acc_t != NULL)
    {
        /*check if we found the account*/
        if(strncmp((const char*)args_Card_t->primaryAccountNumber, (const char*)local_acc_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) == 0)
        {
            /*we found it , YAY*/
            local_errorSate_t = SERVER_OK;
            break;
        }
        /*get the next element in the linked list*/
        local_acc_t = local_acc_t->next;
    }

    /*the account isn't found*/
    if(local_acc_t == NULL)
    {
        local_errorSate_t = ACCOUNT_NOT_FOUND;
    }
    /*check for its state if it's blocked*/
    else if(local_acc_t->state == BLOCKED)
    {
        local_errorSate_t = BLOCKED_ACCOUNT;
    }
    else if(local_acc_t->state == RUNNING)
    {
        local_errorSate_t = SERVER_OK;
    }

    /*return that state*/
    return local_errorSate_t;
}

/*  @fn         :       -blockAccount
 *
 *  @params[0]  :       -a struct to block that account
 *
 *  @brief      :       -blocks an account from being doing any transaction
 *
 *  @notes      :       -none
 *
 *  @return     :       -error state indicating whether that account is running or not
 */
EN_serverError_t blockAccount(ST_cardData_t* arg_Card_t)
{
    /*error state indicating whether we could block the account or not*/
    EN_serverError_t local_errorSate_t = SERVER_OK;

    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    ST_accountsDB_t *local_acc_t = accountsHead.head;

    /*loop all over accounts till get the correct account to get the  account*/
    while(local_acc_t != NULL)
    {
        /*check if we found the account*/
        if(strncmp((const char*)arg_Card_t->primaryAccountNumber, (const char*)local_acc_t->cardHolderData.primaryAccountNumber, ACCOUNT_PAN_SIZE) == 0)
        {
            /*we found it , YAY*/
            local_errorSate_t = SERVER_OK;
            break;
        }
        /*get the next element in the linked list*/
        local_acc_t = local_acc_t->next;
    }   

    /*the account isn't found*/
    if(local_acc_t == NULL)
    {
        local_errorSate_t = ACCOUNT_NOT_FOUND;
    }
    else
    {
        /*change the account status to be blocked*/
        local_acc_t->state = BLOCKED;

        /*update the database*/
        updateAccountDB();
    }

    /*will always return a success*/
    return local_errorSate_t;
}