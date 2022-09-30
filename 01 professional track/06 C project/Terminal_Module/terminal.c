#include "terminal.h"
#include "../colors.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/*************************************************** private function prototypes ****************************************************/


/*************************************************** global variables ****************************************************/

/*these variable are meant to be static to be protected from any modification*/
static float global_f32MaxAmount = 0;
static int8_t global_s8Password[MAX_PASS_LEN]; 



/*  @fn     :       -terminalInit
 *
 *  @params :       -none
 *
 *  @brief  :       -retreive both the password and max amount to be transcated from the database
 *
 *  @return  :      -none
 */
void terminalInit()
{

    /*temp variable used to read from the database line by line*/
    int8_t local_s8ArrDummy[MAX_TRANS_LINE_SIZE_DB];

    /*pointer that points to the file*/
    FILE *file;

    /*open database maxAmount file for read only*/
    file = fopen(MAX_AMOUNT_DB_NAME, "r");
    
    /*read the max amount first and convert it into integer*/
    fgets((char*)local_s8ArrDummy, MAX_TRANS_LINE_SIZE_DB, file);
    global_f32MaxAmount = atof((const char*)local_s8ArrDummy);

    /*read the admin password that is required to change the max limit*/
    fgets((char*)global_s8Password, MAX_TRANS_LINE_SIZE_DB, file);

    /*close database maxAmount file*/
    fclose(file);
}

/*  @fn     :       -getCurrentDate
 *
 *  @params :       -none
 *
 *  @brief  :       -get the current date from the device
 *
 *  @return  :      -string containing the date in the format dd/mm/yyyy
 */
int8_t* getCurrentDate(void)
{   
    /*make the date as static array to avoid destruction of the variable*/
    static int8_t local_arrs8CurrentDate[ACCOUNT_TRANS_DATE_LEN];

    /*get the current system time to a local variable*/
    time_t local_CurrentTime_t = time(NULL);
    struct tm local_time_s = *localtime(&local_CurrentTime_t);

    /*doing some required addition to get the required formula*/
    local_time_s.tm_year += 1900;
    local_time_s.tm_mon += 1;


    /*store the current day into the string*/
    local_arrs8CurrentDate[0] = (local_time_s.tm_mday / 10) + '0';
    local_arrs8CurrentDate[1] = (local_time_s.tm_mday % 10) + '0';
    
    /*sore the separator between number which is '/' */
    local_arrs8CurrentDate[2] = '/';

    /*store the current month into the string*/
    local_arrs8CurrentDate[3] = (local_time_s.tm_mon  / 10) + '0';
    local_arrs8CurrentDate[4] = (local_time_s.tm_mon  % 10) + '0';
    
    /*sore the separator between number which is '/' */
    local_arrs8CurrentDate[5] = '/';

    /*store the current month into the string*/
    local_arrs8CurrentDate[6] = (local_time_s.tm_year  / 1000) + '0';
    local_arrs8CurrentDate[7] = (local_time_s.tm_year  / 100) % 10 + '0';
    local_arrs8CurrentDate[8] = (local_time_s.tm_year  / 10) % 10+ '0';
    local_arrs8CurrentDate[9] = (local_time_s.tm_year  % 10) + '0';

    /*store the null character at the end of the string*/
    local_arrs8CurrentDate[10] = '\0';

    return local_arrs8CurrentDate;
}



/*  @fn     :       -getTransactionDate
 *
 *  @params :       -a structure of type "ST_terminalData_t" to fill it with the date of the transaction and store it in form of string containing the date in the format dd/mm/yyyy
 *
 *  @brief  :       -calls another function to get the current date from the system
 *
 *  @return  :      -the state of the getting the value , it will always by TERMINAL_OK
 */
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    /*copy the current date into the structure*/
    strncpy((char*)termData->transactionDate, (char*)getCurrentDate(), ACCOUNT_TRANS_DATE_LEN + 1);

    /*will always get the correct date of today*/
    return TERMINAL_OK;
}

/*  @fn         :       -isCardExpried
 *
 *  @params[0]  :       -takes the expiry date of the card in form structure to test it
 *
 *  @params[1]  :       -takes the current date of the 
 *
 *  @brief      :       -check whether the given card expirt date makes card valid now or not
 *
 *  @return     :       -the error state of checking whether the card is expired or not
 */
 EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    /*local variable to be returned to indicate whether the card is expired or not*/
    EN_terminalError_t local_enumErroState = TERMINAL_OK;

    /*get the current year and month inform of integers*/
    uint8_t local_u8CurrentYear = 10 * (uint16_t)(termData.transactionDate[8] - '0') + (uint16_t)(termData.transactionDate[9] - '0');
    uint8_t local_u8CurrentMonth = 10 * (uint8_t)(termData.transactionDate[3] - '0') + (uint8_t)(termData.transactionDate[4] - '0');

    /*get the expiry month and year of the card*/
    uint8_t local_u8CardExpYear = 10 * (uint16_t)(cardData.cardExpirationDate[3] - '0') + (uint16_t)(cardData.cardExpirationDate[4] - '0');
    uint8_t local_u8CardExpMonth = 10 * (uint8_t)(cardData.cardExpirationDate[0] - '0') + (uint8_t)(cardData.cardExpirationDate[1] - '0');    

    /*compare the year first and then month , if equal then the card is not expired*/
    if(local_u8CurrentYear > local_u8CardExpYear)
    {
        /*the card is expired if year of transaction is past the expiry date*/
        local_enumErroState = EXPIRED_CARD;
    }
    else if((local_u8CurrentYear == local_u8CardExpYear) && (local_u8CurrentMonth > local_u8CardExpMonth))
    {
        /*the card is expired if year year is same but the month is past expiry month*/
        local_enumErroState = EXPIRED_CARD;
    }
    else
    {
        /*not required but it's here as MISRA C stated that there is must be else , the card is not expired*/
        local_enumErroState = TERMINAL_OK;
    }

    return local_enumErroState;
}

/*  @fn         :       -getTransactionAmount
 *
 *  @params[0]  :       -a structure of type "ST_terminalData_t" to fill in it the required amount to be transacted
 *
 *  @brief      :       -get the amount of the money that the user wants to transact to
 *
 *  @return     :       -the error state of getting that amount from the user
 */
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    /*a buffer used to read what the user entered*/
    int8_t local_arrs8DummyBuff[20];
    
    /*a error state to indicate that the user entered a correct number*/
    EN_terminalError_t local_enumErrorState = TERMINAL_OK;

    /*check if all entered chars are numbers*/
    fputs(BLUE "enter the amount to transact : " YELLOW, stdout);

    /*read what the user entered in a dummy buffer*/
    fgets((char*)local_arrs8DummyBuff, MAX_FLOAT_CHAR_LEN, stdin);

    /*ckech if the entered number is correct*/
    for(int i = 0; local_arrs8DummyBuff[i] != '\0'; i++)
    {
        if(local_arrs8DummyBuff[0] == '\n')
        {
            fputs(RED "you entered nothing , please enter a valid amount\n", stdout);

            /*the user didn't enter any value to be transacted*/
            local_enumErrorState = INVALID_AMOUNT;

            break;                
        }
        else if(local_arrs8DummyBuff[i] == '\n')
        {
            /*converting the entered number to float*/
            local_arrs8DummyBuff[i] = '\0';
            termData->transAmount = atof((const char*) local_arrs8DummyBuff);

            /*the entered value is correct*/
            local_enumErrorState = TERMINAL_OK;

        }
        else if((local_arrs8DummyBuff[i] > '9' || local_arrs8DummyBuff[i] < '0') && local_arrs8DummyBuff[i] != '.')
        {
            fputs(RED "the entered value isn't a number\n", stdout);

            /*the user entered chars instead of numbers*/
            local_enumErrorState = INVALID_AMOUNT;

            break;
        }
        else
        {
            // no operation
        }
    }


    return local_enumErrorState;
}

/*  @fn         :       -isBelowMaxAmount
 *
 *  @params[0]  :       -structure containing amount of the money to be transcated
 *
 *  @brief      :       -checks if the amount of money to be transcated is less than the limit or not
 *
 *  @return     :       -error state indicating whether exceeded maximum amount or not
 */
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    /*a local error state to indicate whether it exceeded the maximum amount or not*/
    EN_terminalError_t local_enumErrorState = TERMINAL_OK;

    /*check whether exceeded the maximum amount or not*/
    if(termData->transAmount > global_f32MaxAmount)
    {   
        local_enumErrorState = EXCEED_MAX_AMOUNT;
    }

    /*return the status*/
    return local_enumErrorState;
}

/*  @fn         :       -setMaxAmount
 *
 *  @params[0]  :       -structure containing the new max amount of money limit to be set on that terminal
 *
 *  @brief      :       -sets a new limit to the money to be transcated
 *
 *  @notes      :       -only admin users are allowed to use this function
 *
 *  @return     :       -error state indicating whether the function setted the new max amount successfully or not 
 */
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
    /*a buffer used to read what the user entered*/
    int8_t local_arrs8DummyBuff[MAX_PASS_LEN];

    /*boolean var to indicate that the entered password is correct or not*/
    EN_terminalError_t local_errorErrorState = TERMINAL_OK;

    /*get the user input*/
    fputs(BLUE "enter the admin password : \n" YELLOW, stdout);
    fgets((char*)local_arrs8DummyBuff, MAX_PASS_LEN, stdin);

    for(uint8_t i = 0; local_arrs8DummyBuff[i] != '\0'; i++)
    {
        if(i != 0 && local_arrs8DummyBuff[i] == '\n')
        {
            /*replacing the \n with \0*/
            local_arrs8DummyBuff[i] = '\0';

            /*he is an adming*/
            local_errorErrorState = TERMINAL_OK;
        }
        else if(local_arrs8DummyBuff[i + 1] == '\0')
        {
            /*he is an adming*/
            local_errorErrorState = TERMINAL_OK;
        }
        else if(local_arrs8DummyBuff[i] != global_s8Password[i])
        {
            fputs(RED "not an admin\n", stdout);

            /*he entered wrong password*/
            local_errorErrorState = INVALID_MAX_AMOUNT;
            break;
        }
        else
        {
            // nothing to do
        }
    }

    /*then update the value in both global variable and in database*/
    if(local_errorErrorState == TERMINAL_OK)
    {
        /*update the global variable*/
        global_f32MaxAmount = termData->maxTransAmount;

        /*update the value in database*/

        /*pointer that points to the file*/
        FILE *file;

        /*open database maxAmount file for write only*/
        file = fopen(MAX_AMOUNT_DB_NAME, "w");

        /*convert the float to array character*/
        uint8_t local_u8Index = sprintf((char*)local_arrs8DummyBuff, MAX_FLOAT_PREC_STORE, global_f32MaxAmount);

        /*put the null char at the end of the float number*/
        local_arrs8DummyBuff[local_u8Index] = '\0';

        /*store the new max amount*/
        fputs((char*)local_arrs8DummyBuff, file);

        /*add a separator between the lines*/
        fputs("\n", file);

        /*store the password as it's deleted*/
        fputs((char*)global_s8Password, file);

        /*close database maxAmount file*/
        fclose(file);

        /*message to indicate that the operation was successful*/
        fputs(GREEN "operation went successfully\n", stdout);
    }

    return local_errorErrorState;
}

/*  @fn         :       -isValidCardPAN
 *
 *  @params[0]  :       -structure containg the PAN to be tested , 16 digits in form of array
 *
 *  @brief      :       -use a simple equation to test that the PAN isn't fake by testing the last 4 digits of the PAN which are generated by equation
 *
 *  @equation   :       -((12_MSB_number * 5) %10) * 1000 + ((12_MSB_number / 2) %10) * 100 + ((((12_MSB_number + 3) * 2) / 5) %10) * 10 + (12_MSB_number + 2) %10
 *
 *  @return     :       -the error state indicating whether it's a correct PAN or not
 */
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    /*local variable to hold the first 12 digits*/
    uint64_t local_u64NumAsInput = 0;

    /*local variable to hold the last 4 digits*/
    uint32_t local_u32NumToTest = 0;

    /*dummy variable to compare the last 4 digits using the equation with what the user entered*/
    uint32_t local_u32NumGenerated = 0;

    /*error state variable as a flag to check if the entered PAN is fake or not*/
    EN_terminalError_t local_enumErrorState = TERMINAL_OK;

    /*get the first 12 digits*/
    for(uint8_t i = 0; i < 12; i++)
    {
        local_u64NumAsInput *= 10;
        local_u64NumAsInput += (uint64_t)(cardData->primaryAccountNumber[i] - '0');
    }

    /*get the last 4 digits*/
    for(uint8_t i = 12; i < 16; i++)
    {
        local_u32NumToTest *= 10;
        local_u32NumToTest += (uint64_t)(cardData->primaryAccountNumber[i] - '0');
    }

    /*generate the correct number*/
    local_u32NumGenerated = ((local_u64NumAsInput * 5) %10) * 1000 + ((local_u64NumAsInput / 2) %10) * 100 + ((((local_u64NumAsInput + 3) * 2) / 5) %10) * 10 + (local_u64NumAsInput + 2) %10;

    /*chechs if the generated number equal to the correct*/
    if(local_u32NumGenerated == local_u32NumToTest)
    {
        /*this PAN is valid*/
        local_enumErrorState = TERMINAL_OK;
    }
    else
    {
        /*this PAN is fake*/
        local_enumErrorState = INVALID_CARD;
    }

    return local_enumErrorState;
}