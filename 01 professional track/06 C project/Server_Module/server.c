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
    /*dummy array that will be converted to a float value later*/
    int8_t local_arrs8DummyArrFloat[MAX_FLOAT_CHAR_LEN];

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
    local_newNode_t->amount = strtof((char*)local_arrs8DummyArrFloat, NULL);    


    /*get the date of transaction */
    strncpy((char*)local_newNode_t->date, (const char*)local_s8Dummy, TRANS_DATE_SIZE);
    local_newNode_t->date[TRANS_DATE_SIZE] = '\0';
    local_s8Dummy += (TRANS_DATE_SIZE + 1);


    /*get the state of the transaction*/
    for(i = 0; *local_s8Dummy != '~'; i++)
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

    /*create a dummy pointer and use it to loop all over the accounts to get reciever*/   
    account_data_node *local_accRec_t = accountsHead.head;

    /*create a dummy pointer and use it to loop all over the accounts to get sender*/   
    account_data_node *local_accSend_t = accountsHead.head;

    /*flag to indicate if there is inComming transactions*/
    _Bool local_boolIsThereTrans = 0;

    /*dummy variable to see if the receiver wants to accept the transaction or not*/ 
    int8_t local_s8IsTransactionAccept = 0;

    /*loop all over accounts till get the correct account to get the receiver account*/
    while(strncmp((const char*)arg_arrs8ReceiverPAN, (const char*)local_accRec_t->PAN, ACCOUNT_PAN_SIZE) != 0)
    {
        local_accRec_t = local_accRec_t->next;
    }

    /*get all incoming transactions*/
    while(local_trans_t != NULL)
    {
        /*check if this transaction is coming to me and it's pending*/
        if(strncmp((const char*)local_trans_t->PAN_to, (const char*)arg_arrs8ReceiverPAN, ACCOUNT_PAN_SIZE) == 0 && 
            strncmp((const char*)local_trans_t->state, (const char*)TRANS_STATE_PEND, TRANS_STATE_SIZE) == 0)
        {
            
            /*get the name of the person who is sending this money*/
            int8_t* local_arrs8SenderName = getSenderName(local_trans_t->PAN_from);

            /*tell the user that there is someone who is seding them a money and ask then for apporval*/
            printf(BLUE "a person with name %s wants to send you %f\n", (char*)local_arrs8SenderName, local_trans_t->amount);
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
                strcpy((char*)local_trans_t->state, TRANS_STATE_COMP);

                /*update the account balance with the new balance*/
                local_accRec_t->balance += local_trans_t->amount;

                fputs(GREEN "operation accepted successfully\n", stdout);

            }
            else if(local_s8IsTransactionAccept == 'n')
            {
                /*make the flag 1 to be used in order to update the database*/
                local_boolIsThereTrans = 1;

                /*make the state of the transaction as refused*/
                strcpy((char*)local_trans_t->state, TRNAS_STATE_REF);

                /*loop all over accounts till get the correct account to get the sender account*/
                while(strncmp((const char*)local_trans_t->PAN_from, (const char*)local_accSend_t->PAN, ACCOUNT_PAN_SIZE) != 0)
                {
                    local_accSend_t = local_accSend_t->next;
                }

                /*return the sender their money back*/
                local_accSend_t->balance +=  local_trans_t->amount;

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
            fputs(RED "inValid holder name\n", stdout);
        }
     
        /*checks for the expiry date*/
        if(arg_account_t->year == local_ptrDummy->year && arg_account_t->month == local_ptrDummy->month)
        {
            local_boolIsValidEXP = 1;
        }  
        else
        {
            fputs(RED "inValid expiry date \n", stdout);
        }

    }
    else
    {
        fputs(RED "the entered PAN is invalid\n" ,stdout);
    }

    return (local_boolIsValidPAN & local_boolIsValidNAME & local_boolIsValidEXP);
}

/*  @fn         :       -isAmountAvailable
 *
 *  @params[0]  :       -an array of size = ACCOUNT_PAN_SIZE which is the PAN of the account to check
*
 *  @params[1]  :       -the amount of money to test if it's available or not
 *
 *  @brief      :       -loops over all accounts and check if the account with same PAN has enought amount or not
 *
 *  @notes      :       -should be called after checking if account isValid
 *
 *  @return     :       - 1 : if the amount is available
 *                        0 : if the amount isn't enough
 */
_Bool isAmountAvailable(int8_t* arg_arrsAccountPAN, float arg_f32amount)
{
    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    account_data_node *local_acc_t = accountsHead.head;

    /*flag to indicate if the amount is available*/
    _Bool local_boolIsAmountAva = 0;

    /*loop all over accounts till get the correct account to get the account*/
    while(strncmp((const char*)arg_arrsAccountPAN, (const char*)local_acc_t->PAN, ACCOUNT_PAN_SIZE) != 0)
    {
        local_acc_t = local_acc_t->next;
    }

    /*checks whether the amount is available*/
    if(arg_f32amount <=  (local_acc_t->balance))
    {
        local_boolIsAmountAva = 1;  // amount is available
    }
    else
    {
        local_boolIsAmountAva = 0;  // amount isn't available
    }

    return local_boolIsAmountAva;
}

/*  @fn         :       -saveTransaction
 *
 *  @params[0]  :       -a structure of type account_transaction_node to be saved in the database
 *
 *  @brief      :       -saves one transaction in the database abd deducts the amount of the money from the sender
 *
 *  @notes      :       -should be called to transafer the amount , also the main program should make sure to fulfil all its params
 *
 *  @return     :       - none
 */
void saveTransaction(account_transaction_node arg_trans_t)
{
    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    account_data_node *local_acc_t = accountsHead.head;    

    /*create a copy of the argument in the heap to avoid its destruction*/
    account_transaction_node *local_trans_t = (account_transaction_node*)malloc(sizeof(account_transaction_node));

    /*loop all over accounts till get the correct account to get the account*/
    while(strncmp((const char*)arg_trans_t.PAN_from, (const char*)local_acc_t->PAN, ACCOUNT_PAN_SIZE) != 0)
    {
        local_acc_t = local_acc_t->next;
    }

    /*subtract the amount to transact*/
    local_acc_t->balance -= arg_trans_t.amount;

    /*copy all memory elements from argument to the created node*/
    memcpy(local_trans_t, &arg_trans_t, sizeof(arg_trans_t));

    /*saves the transaction in the linked list*/
    transactionsHead.tail->next = local_trans_t;
    transactionsHead.tail = local_trans_t;

    /*update the transaction datebase*/
    updateTransactionDB();

    /*update the accounts database*/
    updateAccountDB();
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
    account_transaction_node *local_trans_t = transactionsHead.head;

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
        strncpy((char*)local_s8ArrDummy, (const char*)local_trans_t->PAN_from, ACCOUNT_PAN_SIZE);
        local_u8Index = ACCOUNT_PAN_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*copying the "to PAN number"*/
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_trans_t->PAN_to, ACCOUNT_PAN_SIZE);
        local_u8Index += ACCOUNT_PAN_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';   

        /*saving the amount*/
        local_u8Index += sprintf((char*)(local_s8ArrDummy + local_u8Index), MAX_FLOAT_PREC_STORE, local_trans_t->amount);

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*storing the date*/
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_trans_t->date, TRANS_DATE_SIZE);
        local_u8Index += TRANS_DATE_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*store the state of the transaction*/
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_trans_t->state, strlen((char*)local_trans_t->state));
        local_u8Index += strlen((char*)local_trans_t->state);

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
    account_data_node *local_acct_t = accountsHead.head;

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
        /*<PAN>~<expire_data>~<holder_name>~<balance_amount>~*/

        /*copying the PAN number*/
        strncpy((char*)local_s8ArrDummy, (const char*)local_acct_t->PAN, ACCOUNT_PAN_SIZE);
        local_u8Index = ACCOUNT_PAN_SIZE;

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*copying the month expiry date*/
        local_s8ArrDummy[local_u8Index++] = (int8_t)((local_acct_t->month / 10) + '0');
        local_s8ArrDummy[local_u8Index++] = (int8_t)((local_acct_t->month % 10) + '0');

        /*adding a delimeter between month and year of expiry*/
        local_s8ArrDummy[local_u8Index++] = '/';

        /*copting thr year of expiry*/
        local_s8ArrDummy[local_u8Index++] = (int8_t)((local_acct_t->year / 1000) + '0');
        local_s8ArrDummy[local_u8Index++] = (int8_t)((local_acct_t->year / 100) % 10 + '0');        
        local_s8ArrDummy[local_u8Index++] = (int8_t)((local_acct_t->year / 10) % 10 + '0');   
        local_s8ArrDummy[local_u8Index++] = (int8_t)((local_acct_t->year % 10) + '0');

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~'; 

        /*copying the name of the holder*/  
        strncpy((char*)(local_s8ArrDummy + local_u8Index), (const char*)local_acct_t->name, strlen((char*)local_acct_t->name));
        local_u8Index += strlen((char*)local_acct_t->name);        

        /*put the delimeter '~*/
        local_s8ArrDummy[local_u8Index++] = '~';

        /*copying the balane amount*/
        local_u8Index += sprintf((char*)(local_s8ArrDummy + local_u8Index), MAX_FLOAT_PREC_STORE, local_acct_t->balance);

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
 *  @params[0]  :       -an array of size = ACCOUNT_PAN_SIZE which is PAN of the user
 *
 *  @brief      :       -prints the user current balance
 *
 *  @notes      :       -none
 *
 *  @return     :       -none
 */
void showCurrentBalance(int8_t* arg_arrs8AccPAN)
{
    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    account_data_node *local_accSend_t = accountsHead.head;

    /*loop all over accounts till get the correct account to get the  account*/
    while(strncmp((const char*)arg_arrs8AccPAN, (const char*)local_accSend_t->PAN, ACCOUNT_PAN_SIZE) != 0)
    {
        local_accSend_t = local_accSend_t->next;
    }

    printf(YELLOW "the balance in your account = %f\n", local_accSend_t->balance);

}

/*  @fn         :       -showAllPrvTrans
 *
 *  @params[0]  :       -an array of size = ACCOUNT_PAN_SIZE which is PAN of the user
 *
 *  @brief      :       -prints all previous trans made by this user
 *
 *  @notes      :       -none
 *
 *  @return     :       -none
 */
void showAllPrvTrans(int8_t* arg_arrs8AccPAN)
{
    /*create a dummy pointer and use it to loop all over the transactions*/
    account_transaction_node *local_trans_t = transactionsHead.head;

    /*prints table title*/
    printf(MAGENTA "from\t\t to\t\t  amount\t date\t state\n");

    /*loop all over accounts till get the correct account to get the sender account*/
    while(local_trans_t != NULL)
    {
        /*check if the user got involved in this transaction*/
        if((strncmp((const char*)local_trans_t->PAN_from, (const char*)arg_arrs8AccPAN, ACCOUNT_PAN_SIZE) == 0) 
            || (strncmp((const char*)local_trans_t->PAN_to, (const char*)arg_arrs8AccPAN, ACCOUNT_PAN_SIZE) == 0))
        {
            printf(MAGENTA "%s %s %f %s %s\n",local_trans_t->PAN_from, local_trans_t->PAN_to, local_trans_t->amount, local_trans_t->date, local_trans_t->state);
        }

        /*go to the next node*/
        local_trans_t = local_trans_t->next;
    }
}

/*  @fn         :       -isThatPANThere
 *
 *  @params[0]  :       -an array of size = ACCOUNT_PAN_SIZE which is PAN of the user
 *
 *  @brief      :       -checks whether that reciever is there in the database or not
 *
 *  @notes      :       -none
 *
 *  @return     :       - 1 : if the reciever is there in the database
 *                        0 : if the receiver isn't present
 */
_Bool isThatPANThere(int8_t* arg_arrsAccPAN)
{
    /*a flag to indicate whether the PAN exist or not*/
    _Bool isPANExist = 0;

    /*create a dummy pointer and use it to loop all over the accounts to get the account*/   
    account_data_node *local_acc_t = accountsHead.head;

    /*loop all over accounts till get the correct account to get the  account*/
    while(local_acc_t != NULL)
    {
        /*check if we found the account*/
        if(strncmp((const char*)arg_arrsAccPAN, (const char*)local_acc_t->PAN, ACCOUNT_PAN_SIZE) == 0)
        {
            /*we found it , YAY*/
            isPANExist = 1;
            break;
        }
        /*get the next element in the linked list*/
        local_acc_t = local_acc_t->next;
    }
        
    return isPANExist;
}