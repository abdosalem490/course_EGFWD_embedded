#include "card.h"
#include "../colors.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>


/*  @fn     :       -getCardHolderName
 *
 *  @params :       -none
 *
 *  @brief  :       -take holder name of the card from the user
 *
 *  @return  :      -return a valid but not verified name of the user
 */
int8_t* getCardHolderName()
{
    /*the var name is static to avoid destruction of the local variable*/
    static int8_t name[ACCOUNT_NAME_SIZE];

    /*flag to indicate that the entered name isn't empty*/
    _Bool local_boolIsValid = 0;

    do{
        /*ask for user input*/
        puts(CYAN "enter your name : " YELLOW);
        fgets((char*)name, ACCOUNT_NAME_SIZE, stdin);

        /*used to clear the buffer to avoid more characters there in the buffer*/
        fflush(stdin);

        /*check the validity of the input*/
        for(uint8_t i = 0; name[i] != '\0'; i++)
        {
            /*if there is a terminating char in the first 4 chars*/
            if(i < ACCOUNT_MIN_NAME_LEN && name[i] == '\0')
            {
                printf(RED "invalid name , minmum name size must be %d\n", ACCOUNT_MIN_NAME_LEN);
                break;
            }
            /*if the name contain numbers*/
            else if(name[i] > '0' && name[i] < '9')
            {
                printf(RED "invalid name , there is no name with digits in it\n");
                break;   
            }
            else if(name[i] == '\n')
            {
                local_boolIsValid = 1;

                /*put a terminating char instead of the line feed one*/
                name[i] = '\0';

            }
        }
    }while(!local_boolIsValid);

    return name;
}

/*  @fn     :       -getCardExpiryDate
 *
 *  @params :       -none
 *
 *  @brief  :       -take holder card expiry date from the user
 *
 *  @return  :      -return a valid but not verified expiry date of the card
 */
int8_t* getCardExpiryDate()
{
    /*the var local_arrs8ExpiryDate is static to avoid destruction of the local variable during exit of the function*/
    static int8_t local_arrs8ExpiryDate[ACCOUNT_EXPIRY_DATE_LEN + 1];

    /*flag to indicate that the entered local_arrs8ExpiryDate isn't empty*/
    _Bool local_boolIsValid = 0;

    /*get the month of expiry from the user and verify it*/
    do{
        /*ask for user input*/
        puts(CYAN "enter month of expiry date (2 or 1 digits) : " YELLOW);
        fgets((char*)local_arrs8ExpiryDate, ACCOUNT_EXPIRY_MON_LEN + 2, stdin);

        /*used to clear the buffer to avoid more characters there in the buffer*/
        fflush(stdin);

        /*check the validity of the input (month) to be 1 or 2 digits and only digits*/

        /*if there is a line feed char in the first 1 chars*/
        if(local_arrs8ExpiryDate[0] == '\n')
        {
            printf(RED "no month date entered\n");
        }
        /*first char must be a digit*/
        else if(local_arrs8ExpiryDate[0] > '9' || local_arrs8ExpiryDate[0] < '0')
        {
            printf(RED "month must be only valid digits\n");
        }
        /*second char can be a digit or just line feed*/
        else if(local_arrs8ExpiryDate[1] != '\n' && (local_arrs8ExpiryDate[1] > '9' || local_arrs8ExpiryDate[1] < '0'))
        {
            printf(RED "month must be only digits and not more than 12\n");           
        }
        else if(local_arrs8ExpiryDate[1] == '\n' && local_arrs8ExpiryDate[0] <= '9' && local_arrs8ExpiryDate[0] > '0')
        {
            local_boolIsValid = 1;

            /*make the month format in the form 0m/yyyy */
            local_arrs8ExpiryDate[1] = local_arrs8ExpiryDate[0];
            local_arrs8ExpiryDate[0] = '0';
        }
        else
        {
            /*check if it's a valid number between 0 and 12*/
            uint8_t local_u8DummyTest = 10 * (uint8_t)(local_arrs8ExpiryDate[0] - '0') + (uint8_t)(local_arrs8ExpiryDate[1] - '0');
            if(local_u8DummyTest < 13)
            {
                local_boolIsValid = 1;
            }
            else
            {
                printf(RED "there is no month greater than 12\n");
            }
        }
    }while(!local_boolIsValid);

    /*put the separator between the month and the year*/
    local_arrs8ExpiryDate[ACCOUNT_EXPIRY_MON_LEN] = '/';

    /*reset the flag*/
    local_boolIsValid = 0;

    /*get the month of expiry from the user and verify it*/
    do{
        /*ask for user input*/
        puts(CYAN "enter year of expiry date (4 digits) : " YELLOW);
        fgets((char*)(local_arrs8ExpiryDate + ACCOUNT_EXPIRY_MON_LEN + 1), (ACCOUNT_EXPIRY_YEAR_LEN + 2), stdin);

        /*used to clear the buffer to avoid more characters there in the buffer*/
        fflush(stdin);

        /*check the validity of the input (year) to be 4 digits only*/
        for(uint8_t i = (ACCOUNT_EXPIRY_MON_LEN + 1); i < (ACCOUNT_EXPIRY_DATE_LEN - 1); i++)
        {
            /*if there is a line feed char in the chars*/
            if(local_arrs8ExpiryDate[i] == '\n')
            {
                printf(RED "entered year expiry date is invalid, it must be 4 digits \n");
                break;
            }
            else if(local_arrs8ExpiryDate[i] > '9' || local_arrs8ExpiryDate[i] < '0')
            {
                printf(RED "year must be only digits\n");
                break;
            }
            else if(i == (ACCOUNT_EXPIRY_DATE_LEN - 2))
            {
            
                /*convert entered string to number*/
                uint16_t local_u8DummyTest = 0;
                for(uint8_t j = (ACCOUNT_EXPIRY_MON_LEN + 1); j < (ACCOUNT_EXPIRY_DATE_LEN - 1); j++)
                {
                    local_u8DummyTest *= 10;
                    local_u8DummyTest += (uint16_t)(local_arrs8ExpiryDate[j] - '0');
                }

                /*check if the entered year is between 2100 and 2000*/
                if(local_u8DummyTest < 2100 && local_u8DummyTest > 2000)
                {
                    local_boolIsValid = 1;
                }
                else
                {
                    printf(RED "entered year range must be between 2100 and 2000\n");
                }
            }
        }
    }while(!local_boolIsValid);   

    /*terminate the string with \0 instead of \0*/
    local_arrs8ExpiryDate[ACCOUNT_EXPIRY_DATE_LEN - 1] = '\0';

    return local_arrs8ExpiryDate;    
}

/*  @fn     :       -getCardPAN
 *
 *  @params :       -none
 *
 *  @brief  :       -take holder peimary account number from the user and the entered value must be all numbers
 *
 *  @return  :      -return a valid but not verified Primary account number of the card
 */
int8_t* getCardPAN()
{
    /*the var local_arrs8PAN is static to avoid destruction of the local variable*/
    static int8_t local_arrs8PAN[ACCOUNT_PAN_SIZE + 2];

    /*flag to indicate that the entered PAN isn't empty and also valid*/
    _Bool local_boolIsValid = 0;

    do{
        /*ask for user input*/
        puts(CYAN "enter your PAN (16 valid digit numbers) : " YELLOW);
        fgets((char*)local_arrs8PAN, (ACCOUNT_PAN_SIZE + 1), stdin);

        /*used to clear the buffer to avoid more characters there in the buffer*/
        fflush(stdin);

        /*check the validity of the input*/
        for(uint8_t i = 0; i < (ACCOUNT_PAN_SIZE + 1); i++)
        {
            /*if there is a terminating char in the first char*/
            if(i == 0 && local_arrs8PAN[i] == '\0')
            {
                printf(RED "NO PAN is entered\n");
                break;
            }
            /*the PAN length must be 16*/
            else if(i < ACCOUNT_PAN_SIZE && local_arrs8PAN[i] == '\n')
            {
                printf(RED "invalid PAN , PAN length must be exactly 16 digits\n");
                break;                       
            }
            /*the PAN must only conatians numbers*/
            else if((local_arrs8PAN[i] < '0' || local_arrs8PAN[i] > '9') && i < ACCOUNT_PAN_SIZE)
            {
                printf(RED "invalid PAN , the PAN must only contain digits\n");
                break;   
            }
            else if(local_arrs8PAN[i] == '\n' || local_arrs8PAN[i] == '\0')
            {
                local_boolIsValid = 1;

                /*put a terminating char instead of the line feed one*/
                local_arrs8PAN[i] = '\0';
            }
        }
    }while(!local_boolIsValid);

    return local_arrs8PAN;    
}

/*  @fn     :       -getRecieverAccountPAN
 *
 *  @params :       -none
 *
 *  @brief  :       -take reciever peimary account number from the user and the entered value must be all numbers
 *
 *  @return  :      -return a valid but not verified Primary account number of the card of the person to transmit money to
 */
int8_t* getRecieverAccountPAN(void)
{
    /*the var local_arrs8PAN is static to avoid destruction of the local variable*/
    static int8_t local_arrs8PAN[ACCOUNT_PAN_SIZE + 2];

    /*flag to indicate that the entered PAN isn't empty and also valid*/
    _Bool local_boolIsValid = 0;

    do{
        /*ask for user input*/
        puts(CYAN "enter reciever PAN (16 valid digit numbers) : " YELLOW);
        fgets((char*)local_arrs8PAN, (ACCOUNT_PAN_SIZE + 1), stdin);

        /*used to clear the buffer to avoid more characters there in the buffer*/
        fflush(stdin);

        /*check the validity of the input*/
        for(uint8_t i = 0; i < (ACCOUNT_PAN_SIZE + 1); i++)
        {
            /*if there is a terminating char in the first char*/
            if(i == 0 && local_arrs8PAN[i] == '\0')
            {
                printf(RED "NO PAN is entered\n");
                break;
            }
            /*the PAN length must be 16*/
            else if(i < ACCOUNT_PAN_SIZE && local_arrs8PAN[i] == '\n')
            {
                printf(RED "invalid PAN , PAN length must be exactly 16 digits\n");
                break;                       
            }
            /*the PAN must only conatians numbers*/
            else if((local_arrs8PAN[i] < '0' || local_arrs8PAN[i] > '9') && i < ACCOUNT_PAN_SIZE)
            {
                printf(RED "invalid PAN , the PAN must only contain digits\n");
                break;   
            }
            else if(local_arrs8PAN[i] == '\n' || local_arrs8PAN[i] == '\0')
            {
                local_boolIsValid = 1;

                /*put a terminating char instead of the line feed one*/
                local_arrs8PAN[i] = '\0';
            }
        }
    }while(!local_boolIsValid);

    return local_arrs8PAN;    
}