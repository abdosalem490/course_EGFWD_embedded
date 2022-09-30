#include "card.h"
#include "../colors.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>


/*  @fn     :       -getCardHolderName
 *
 *  @params :       -a structure to fill in it the name of the card holder
 *
 *  @brief  :       -take holder name of the card from the user and it gets a valid but not verified name of the user
 *
 *  @return  :      -the state of the function whether it successfully get the name or not
 */
EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    /*local variable to store the error state of getting the name*/
    EN_cardError_t local_enumErrorState = CARD_OK;

    /*ask for user input*/
    puts(CYAN "enter your name : " YELLOW);
    fgets((char*)cardData->cardHolderName, ACCOUNT_NAME_SIZE, stdin);

    /*used to clear the buffer to avoid more characters there in the buffer*/
    fflush(stdin);

    /*check the validity of the input*/
    for(uint8_t i = 0; cardData->cardHolderName[i] != '\0'; i++)
    {
        /*if there is a terminating char in the first 4 chars*/
        if(i < ACCOUNT_MIN_NAME_LEN  && cardData->cardHolderName[i] == '\n')
        {
            printf(RED "invalid name , minmum name size must be %d\n", ACCOUNT_MIN_NAME_LEN);

            /*the name is less than minum length for the name*/
            local_enumErrorState = WRONG_NAME;

            break;
        }
        /*if the name contain numbers*/
        else if(cardData->cardHolderName[i] > '0' && cardData->cardHolderName[i] < '9')
        {
            printf(RED "invalid name , there is no name with digits in it\n");

            /*the name contains numbers*/
            local_enumErrorState = WRONG_NAME;

            break;   
        }
        else if(cardData->cardHolderName[i] == '\n')
        {
            /*the name is stored correctly*/
            local_enumErrorState = CARD_OK;
    
            /*put a terminating char instead of the line feed one*/
            cardData->cardHolderName[i] = '\0';

        }
    }

    return local_enumErrorState;
}

/*  @fn     :       -getCardExpiryDate
 *
 *  @params :       -a structure of type "ST_cardData_t" to fill in it the expory date of the card holder
 *
 *  @brief  :       -take expiry date from the user and get a valid but not verified expiry date of the card
 *
 *  @return  :      -return the error state of getting the expiry date
 */
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    /*local variable to store the error state of getting the expiry date*/
    EN_cardError_t local_enumErrorState = CARD_OK;

    /*used to test for the entered month*/
    uint8_t local_u8DummyTest = 0;

    /*get the expiry date from the user and verify it*/
    puts(CYAN "enter month and year of expiry date in the form of MM/YY : " YELLOW);
    fgets((char*)cardData->cardExpirationDate, ACCOUNT_EXPIRY_DATE_LEN + 1, stdin);

    /*used to clear the buffer to avoid more characters there in the buffer*/
    fflush(stdin);

    /*if there is a line feed char in the first 1 chars*/
    if(cardData->cardExpirationDate[0] == '\n')
    {
        printf(RED "no date entered\n");

        /*the entered date is invalid*/
        local_enumErrorState = WRONG_EXP_DATE;        
    }
    /*most of chars must be digits*/
    else if((cardData->cardExpirationDate[0] > '9' || cardData->cardExpirationDate[0] < '0' || cardData->cardExpirationDate[1] > '9' || cardData->cardExpirationDate[1] < '0'
            || cardData->cardExpirationDate[3] > '9' || cardData->cardExpirationDate[3] < '0' || cardData->cardExpirationDate[4] > '9' || cardData->cardExpirationDate[4] < '0')
            || cardData->cardExpirationDate[2] != '/')
    {
        printf(RED "month must be only valid digits and in the form of MM/YY\n");

        /*the entered date is invalid*/
        local_enumErrorState = WRONG_EXP_DATE;
    }
    else
    {
        /*check if month is a valid number between 0 and 12*/
        local_u8DummyTest = 10 * (uint8_t)(cardData->cardExpirationDate[0] - '0') + (uint8_t)(cardData->cardExpirationDate[1] - '0');
        if(local_u8DummyTest < 13 && local_u8DummyTest > 0)
        {
            /*the entered date is invalid*/
            local_enumErrorState = CARD_OK;
        }
        else
        {
            printf(RED "there is no month greater than 12\n");

            /*the entered date is invalid*/
            local_enumErrorState = WRONG_EXP_DATE;
        }
    }

    return local_enumErrorState;    
}

/*  @fn     :       -getCardPAN
 *
 *  @params :       -a strucutre of type "ST_cardData_t" to fill it with the PAN of the user
 *
 *  @brief  :       -take holder peimary account number from the user and the entered value must be all numbers and get a valid but not verified Primary account number of the card
 *
 *  @return  :      -return the error state of getting the card PAN
 */
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    /*local variable to store the error state of getting the PAN*/
    EN_cardError_t local_enumErrorState = CARD_OK;

    /*ask for user input*/
    puts(CYAN "enter PAN (16 valid digit numbers) : " YELLOW);
    fgets((char*)cardData->primaryAccountNumber, (ACCOUNT_PAN_SIZE + 1), stdin);

    /*used to clear the buffer to avoid more characters there in the buffer*/
    fflush(stdin);

    /*check the validity of the input*/
    for(uint8_t i = 0; i < (ACCOUNT_PAN_SIZE + 1); i++)
    {
        /*if there is a terminating char in the first char*/
        if(i == 0 && cardData->primaryAccountNumber[i] == '\0')
        {
            printf(RED "NO PAN is entered\n");

            /*no entered PAN*/
            local_enumErrorState = WRONG_PAN;

            break;
        }
        /*the PAN length must be 16*/
        else if(i < ACCOUNT_PAN_SIZE && cardData->primaryAccountNumber[i] == '\n')
        {
            printf(RED "invalid PAN , PAN length must be exactly 16 digits\n");

            /*no entered PAN*/
            local_enumErrorState = WRONG_PAN;

            break;                       
        }
        /*the PAN must only conatians numbers*/
        else if((cardData->primaryAccountNumber[i] < '0' || cardData->primaryAccountNumber[i] > '9') && i < ACCOUNT_PAN_SIZE)
        {
            printf(RED "invalid PAN , the PAN must only contain digits\n");

            /*no entered PAN*/
            local_enumErrorState = WRONG_PAN;

            break;   
        }
        else if(cardData->primaryAccountNumber[i] == '\0')
        {
            /*the entered PAN is correct*/
            local_enumErrorState = CARD_OK;
        }
    }

    return local_enumErrorState;    
}
