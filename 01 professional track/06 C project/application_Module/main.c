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
                            "[3] : make a new transaction\n"
                            "[4] : block your account\n";

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
    
    /*user card data*/
    ST_cardData_t local_senderCard_t;

    /*the card data of the receiver if the user wants to transact*/
    ST_cardData_t local_receiverCard_t;

    /*the transaction data*/
    ST_terminalData_t local_transactionData_t;

    /*initialize the transaction date with today*/
    strcpy((char*)local_transactionData_t.transactionDate, (char*)getCurrentDate());

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
            if(scanf("%f", &local_transactionData_t.maxTransAmount) == 1)
            {
                /*flust the input buffer*/
                fflush(stdin);

                /*try to set the new max amount and check if he is correctly admin*/
                if(setMaxAmount(&local_transactionData_t) == TERMINAL_OK)
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
            while(getCardPAN(&local_senderCard_t) != CARD_OK);

            /*gets the user card holder name*/
            while(getCardHolderName(&local_senderCard_t) != CARD_OK);

            /*gets the user card expiry date*/
            while(getCardExpiryDate(&local_senderCard_t) != CARD_OK);

            /*checks if the PAN number is fake*/
            if(isValidCardPAN(&local_senderCard_t) != TERMINAL_OK)
            {
                fputs(RED "PAN is fake\n", stdout);
            }
            /*check if the card isn't expired*/
            else if(isCardExpired(local_senderCard_t, local_transactionData_t) != TERMINAL_OK)
            {
                fputs(RED "card is expired\n", stdout);
            }
            /*the card input values are valid but not authentitcated*/
            else
            {
                fputs(MAGENTA "checking credentials with the server\n", stdout);
                
                /*checks if the account is valid by connecting to the server*/
                if(isValidAccount(local_senderCard_t) != SERVER_OK)
                {
                    fputs(RED "the account isn't there in the database\n", stdout);
                }
                /*the account is valid*/
                else
                {
                    if(isBlockedAccount(&local_senderCard_t) == BLOCKED_ACCOUNT)
                    {
                        fputs(RED "your account is blocked , you cannot sign in \n", stdout);
                    }
                    else
                    {
                        fputs(GREEN "signed in successfully\n", stdout);

                        /*checks if there is any pending transactions waiting for this account approval*/
                        recieveTransactionData(&local_senderCard_t);

                        do{
                            /*show the user a list of choices to choose from*/
                            printf(MAGENTA "%s" YELLOW,userListChoice);

                            /*get the user choice*/
                            scanf("%c", &local_u8DummyFlag);

                            /*flush the buffer*/
                            fflush(stdin);

                            /*invalid choice*/
                            if(!(local_u8DummyFlag == '1' || local_u8DummyFlag == '2' || local_u8DummyFlag == '3' || local_u8DummyFlag == '4'))
                            {
                                fputs(RED "invalid choice \n", stdout);
                            }
                            /*valid input*/
                            else if(local_u8DummyFlag == '1') 
                            {   
                                /*show your current balance*/
                                showCurrentBalance(&local_senderCard_t);
                            }
                            else if(local_u8DummyFlag == '2')
                            {
                                /*show all previouse transactions*/
                                showAllPrvTrans(&local_senderCard_t);
                            }
                            else if(local_u8DummyFlag == '3')
                            {
                                /*make a new transaction*/

                                /*ask the user for the PAN to transact to*/
                                puts(CYAN "receiver PAN : ");
                                while(getCardPAN(&local_receiverCard_t) != CARD_OK);

                                /*get entered number from the user*/
                                while(getTransactionAmount(&local_transactionData_t) != TERMINAL_OK);

                                /*check if that PAN exists in the database*/
                                if(isThatPANThere(&local_receiverCard_t) != SERVER_OK)
                                {
                                    fputs(RED "there is no one in our database with this PAN \n", stdout);
                                }
                                /*check if the receiver account is blocked or not*/
                                else if(isBlockedAccount(&local_receiverCard_t) == BLOCKED_ACCOUNT)
                                {
                                    fputs(RED "the receiver account is blocked , you cann't send anything to him not receiver from him \n", stdout);
                                }
                                /*check if there is enough money to be transacted*/
                                else if(isAmountAvailable(&local_transactionData_t, &local_senderCard_t) != SERVER_OK)
                                {
                                    fputs(RED "there is not enough money in your account \n", stdout);
                                }
                                /*checks if the transacted money exceeded the limit*/
                                else if(isBelowMaxAmount(&local_transactionData_t) != TERMINAL_OK)
                                {
                                    fputs(RED "you exceeded the maximum allowed limit to transact \n", stdout);
                                }
                                /*all's good , save the transaction*/
                                else
                                {
                                    /*create a transaction*/
                                    ST_transaction *local_transaction_t = (ST_transaction*)malloc(sizeof(ST_transaction));
                                    local_transaction_t->cardHolderDataFrom = local_senderCard_t;
                                    local_transaction_t->cardHolderDataTo = local_receiverCard_t;
                                    local_transaction_t->terminalData = local_transactionData_t;
                                    local_transaction_t->transState = PENDING;

                                    /*save the transaction*/
                                    saveTransaction(local_transaction_t);

                                    /*tell the user that the operation went successfully*/
                                    fputs(GREEN "the operation went successfully \n", stdout);
                                }
                                                        
                            }
                            else if(local_u8DummyFlag == '4')
                            {
                                /*block my account*/
                                if(blockAccount(&local_senderCard_t) == SERVER_OK)
                                {
                                    fputs(GREEN "account blocked successfully and signing out now \n", stdout);

                                    /*to sign out*/
                                    break;
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

    /*print the date of Today*/
    printf(GREEN "======================================================================================================================================================\n");
    printf(GREEN "today is %s\n", getCurrentDate());
}