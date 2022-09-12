#include "../Server_Module/server.h"
#include "../Terminal_Module/terminal.h"
#include "../Card_Module/card.h"
#include "main.h"
#include "../colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*these are just messy variables for showing off*/
char welcome[5][200] = { "##    ####     ##       ########           ##               #########         ##########                ###     ###           ########",   
                         "##    ####     ##       ##                 ##               ##                ##      ##               ## ##   ##  ##         ##",
                         "##    ####    ##        ########           ##               ##                ##      ##              ##   ## ##    ##        ########",
                         " ##  ##  ##  ##         ##                 ##               ##                ##      ##             ##     ###     ##        ##",
                         "  ###     ###           ########           ########         #########         ##########             ##     ###     ##        ########"}; 

char credits1[] = "all credits goes to abdosalm555@gmail.com | @stack_overflow_account : https://stackoverflow.com/users/16305340/abdo-salm\n";
char credits2[] = "@github_account : https://github.com/abdosalem490 | @linkedin_account : https://www.linkedin.com/in/abdo-salm-11a16821a/\n";

const char userListChoice[] = "choose one of the following :\n"
                            "[1] : show your balance account \n"
                            "[2] : show all your previous transactions \n"
                            "[3] : make a new transaction\n";

int main()
{
/*******************************************/
    /*just something to show off*/
    for(int i = 0; i < 150; i++)
        printf(MAGENTA "=");
    printf("\n"); 
    for(int i = 0; i < 5; i++)
        printf("%s\n",welcome[i]);
    for(int i = 0; i < 150; i++)
        printf("=");
    printf("\n"); 
    printf(YELLOW "%s",credits1);
    for(int i = 0; i < 150; i++)
        printf("-");
    printf("\n%s",credits2);
    for(int i = 0; i < 150; i++)
        printf("=");
    printf(WHITE "\n");    
/*******************************************/

    /*begin of application after initialization*/

    /*flag to exit the infinite loop*/
    int8_t local_u8DummyFlag;

    /*the new amount the user would enter to set the max amount limit*/
    float lcoal_f32NewMaxAmount;
    
    /*user card PAN number*/
    int8_t local_arrs8SendPAN[ACCOUNT_PAN_SIZE];

    /*user card holder name*/
    int8_t local_arrs8SendName[ACCOUNT_NAME_SIZE];

    /*user card Expiry date*/
    int8_t local_arrs8SendExpDate[ACCOUNT_EXPIRY_DATE_LEN];

    /*the other account PAN number to transact to*/
    int8_t local_arrs8RcvPAN[ACCOUNT_PAN_SIZE + 1];

    /*the amount to transact*/
    float local_f32AmountToTransact = 0;

    /*dummy account node to put it as args to the server functions*/
    account_data_node local_Dummyacct_t;

    /*dummy transaction node to put it as args to the server functions*/
    account_transaction_node local_dummyTrans_t;

    /*infinty loop as long as the user wants to do many things*/
    do
    {
        /*ask the user if he wanted to change max limit but on condition that he is admin*/
        fputs(BLUE "are you an admin and wants to change the current max limit ?\n", stdout);
        fputs(CYAN "press [y] to change terminal max amount , [n] to enter the user data\n", stdout);
        scanf("%c", &local_u8DummyFlag);

        /*flust the input buffer*/
        fflush(stdin);

        /*try to sets the new max limit*/
        if(local_u8DummyFlag == 'y')
        {
            /*ask the user to enter the new amount*/
            fputs(BLUE "enter the new max limit to set\n", stdout);

            /*get the amount to be set and make sure it's a correct float*/
            if(scanf("%f", &lcoal_f32NewMaxAmount) == 1)
            {
                /*flust the input buffer*/
                fflush(stdin);

                /*try to set the new max amount and check if he is correctly admin*/
                if(setMaxAmount(lcoal_f32NewMaxAmount) == 1)
                {
                    fputs(GREEN "new amount set successfully\n", stdout);
                }
                /*he is not an admin*/
                else
                {
                    fputs(RED "password is incorrect\n", stdout);
                }
            } 
            /*print error message telling him not a valid input*/
            else
            {
                fputs(RED "invalid input\n", stdout);

                /*flust the input buffer*/
                fflush(stdin);
            }
            
        }
        /*get the user data and preform transaction actions*/
        else if(local_u8DummyFlag == 'n')
        {
            fputs(WHITE "---------------------------------------------------------------------------------------------------------------------\n", stdout);

            /*gets the user card PAN*/
            strncpy((char*)local_arrs8SendPAN , (char*)getCardPAN(), ACCOUNT_PAN_SIZE);

            /*gets the user card holder name*/
            strncpy((char*)local_arrs8SendName , (char*)getCardHolderName(), ACCOUNT_NAME_SIZE);

            /*gets the user card expiry date*/
            strncpy((char*)local_arrs8SendExpDate , (char*)getCardExpiryDate(), ACCOUNT_EXPIRY_DATE_LEN);

            /*checks if the PAN number is fake*/
            if(isFakePAN(local_arrs8SendPAN) == 1)
            {
                fputs(RED "PAN is fake\n", stdout);
            }
            /*check if the card isn't expired*/
            else if(isCardExpried(local_arrs8SendExpDate) == 1)
            {
                fputs(RED "card is expired\n", stdout);
            }
            /*the card input values are valid but not authentitcated*/
            else
            {
                fputs(MAGENTA "checking credentials with the server\n", stdout);
                
                /*assging the value PAN to the account dummy node*/
                strncpy((char*)local_Dummyacct_t.PAN , (char*)local_arrs8SendPAN, ACCOUNT_PAN_SIZE);

                /*assging the value holder name to the account dummy node*/
                strncpy((char*)local_Dummyacct_t.name , (char*)local_arrs8SendName, ACCOUNT_NAME_SIZE);

                /*assging the value expiry date to the account dummy node*/
                local_Dummyacct_t.month = 10 * (uint8_t)(local_arrs8SendExpDate[0] - '0') + (uint8_t)(local_arrs8SendExpDate[1] - '0');
                local_Dummyacct_t.year = 1000 * (uint16_t)(local_arrs8SendExpDate[3] - '0') + 100 * (uint16_t)(local_arrs8SendExpDate[4] - '0') + 10 * (uint16_t)(local_arrs8SendExpDate[5] - '0') + (uint16_t)(local_arrs8SendExpDate[6] - '0');

                /*checks if the account is valid by connecting to the server*/
                if(isValidAccount(&local_Dummyacct_t) == 0)
                {
                    fputs(RED "the account isn't there in the database\n", stdout);
                }
                /*the account is valid*/
                else
                {
                    fputs(GREEN "signed in successfully\n", stdout);

                    /*checks if there is any pending transactions waiting for this account approval*/
                    recieveTransactionData(local_arrs8SendPAN);

                    do{
                        /*show the user a list of choices to choose from*/
                        printf(MAGENTA "%s" YELLOW,userListChoice);

                        /*get the user choice*/
                        scanf("%c", &local_u8DummyFlag);

                        /*flush the buffer*/
                        fflush(stdin);

                        /*invalid choice*/
                        if(!(local_u8DummyFlag == '1' || local_u8DummyFlag == '2' || local_u8DummyFlag == '3'))
                        {
                            fputs(RED "invalid choice \n", stdout);
                        }
                        /*valid input*/
                        else if(local_u8DummyFlag == '1') 
                        {   
                            /*show your current balance*/
                            showCurrentBalance(local_arrs8SendPAN);
                        }
                        else if(local_u8DummyFlag == '2')
                        {
                            /*show all previouse transactions*/
                            showAllPrvTrans(local_arrs8SendPAN);
                        }
                        else if(local_u8DummyFlag == '3')
                        {
                            /*make a new transaction*/

                            /*ask the user for the PAN to transact to*/
                            strncpy((char*)local_arrs8RcvPAN , (char*)getRecieverAccountPAN(), ACCOUNT_PAN_SIZE);

                            /*get entered number from the user*/
                            local_f32AmountToTransact = gatTransactionAmount();

                            /*check if that PAN exists in the database*/
                            if(isThatPANThere(local_arrs8RcvPAN) == 0)
                            {
                                fputs(RED "there is no one in our database with this PAN \n", stdout);
                            }
                            /*check if there is enough money to be transacted*/
                            else if(isAmountAvailable(local_arrs8SendPAN, local_f32AmountToTransact) == 0)
                            {
                                fputs(RED "there is not enough money in your account \n", stdout);
                            }
                            /*checks if the transacted money exceeded the limit*/
                            else if(isBelowMaxAmount(local_f32AmountToTransact) == 0)
                            {
                                fputs(RED "you exceeded the maximum allowed limit to transact \n", stdout);
                            }
                            /*all's good , save the transaction*/
                            else
                            {
                                /*create a transaction*/
                                strncpy((char*)local_dummyTrans_t.PAN_from , (char*)local_arrs8SendPAN, ACCOUNT_PAN_SIZE);
                                strncpy((char*)local_dummyTrans_t.PAN_to , (char*)local_arrs8RcvPAN, ACCOUNT_PAN_SIZE);
                                strncpy((char*)local_dummyTrans_t.date , (char*)getTransactionDate(), TRANS_DATE_SIZE);
                                strncpy((char*)local_dummyTrans_t.state , (char*)TRANS_STATE_PEND, strlen(TRANS_STATE_PEND) + 1);
                                local_dummyTrans_t.amount = local_f32AmountToTransact;

                                /*save the transaction*/
                                saveTransaction(local_dummyTrans_t);

                                /*tell the user that the operation went successfully*/
                                fputs(GREEN "the operation went successfully \n", stdout);
                            }
                                                       
                        }
                        else
                        {
                            /*unreachable*/
                        }

                        /*ask the user if he is done or he wants to do anything else*/
                        fputs(BLUE "are you done ?\n", stdout);
                        fputs(CYAN "press [y] to sign out , [n] to return to the list\n", stdout);
                        scanf("%c", &local_u8DummyFlag);

                        /*flush the buffer*/
                        fflush(stdin);

                    }while(local_u8DummyFlag == 'n');

                }
            }

            fputs(WHITE "---------------------------------------------------------------------------------------------------------------------\n", stdout);
        }
        
        
        /*ask the user at the end if he wants to start the whole operation from the begining again*/
        fputs(BLUE "do you want start from all over again ?\n", stdout);
        fputs(CYAN "press [y] to start agian , [n] to end\n", stdout);
        scanf("%c", &local_u8DummyFlag);

        /*flush the stdin as '\n' will be still there in the buffer*/
        fflush(stdin);

        printf(WHITE);
    }while(local_u8DummyFlag == 'y');

    return 0;
}


/*  @fn     :       -appStart
 *
 *  @params :       -none
 *  @notes  :       -this function is get called before calling the main due to 
 *                    the function attribute called constructor
 *  @brief  :       -this function do all required initializations like initializing the server
 *                   to read all data in the database and so on...
 */
void __attribute__ ((constructor)) appStart() 
{
    /*get all the data stored in the database*/
    serverInit();

    /*get max amount limit to be transacted and password*/
    terminalInit();
}