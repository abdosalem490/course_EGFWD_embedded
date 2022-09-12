#include "terminal.h"
#include "../colors.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/*************************************************** private function prototypes ****************************************************/
static int8_t* getCurrentDate(void);                                                                /*get the current date from the device system*/


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
    int8_t local_s8ArrDummy[MAX_LINE_SIZE_DB];

    /*pointer that points to the file*/
    FILE *file;

    /*open database maxAmount file for read only*/
    file = fopen(MAX_AMOUNT_DB_NAME, "r");
    
    /*read the max amount first and convert it into integer*/
    fgets((char*)local_s8ArrDummy, MAX_LINE_SIZE_DB, file);
    global_f32MaxAmount = atof((const char*)local_s8ArrDummy);

    /*read the admin password that is required to change the max limit*/
    fgets((char*)global_s8Password, MAX_LINE_SIZE_DB, file);

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
static int8_t* getCurrentDate(void)
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
 *  @params :       -none
 *
 *  @brief  :       -calls another function to get the current date from the system
 *
 *  @return  :      -string containing the date in the format dd/mm/yyyy
 */
int8_t* getTransactionDate()
{
    return getCurrentDate();
}

/*  @fn         :       -isCardExpried
 *
 *  @params[0]  :       -takes the expiry date of the card in form of mm/yyyy with arra of size 8
 *
 *  @brief      :       -check whether the given card expirt date makes card valid now or not
 *
 *  @return     :       - 1 : if the card is expired
 *                        0 : if the card is still valid
 */
_Bool isCardExpried(int8_t arg_arrs8ExpiryDate[ACCOUNT_EXPIRY_DATE_LEN])
{
    /*flag used to show if it's expired or not*/
    _Bool local_boolIsExpired = 0;

    /*get the current date*/
    int8_t *local_arrs8CurrentDate =  getCurrentDate();

    /*get the current year and month inform of integers*/
    uint16_t local_u16CurrentYear =  1000 * (uint16_t)(local_arrs8CurrentDate[6] - '0') + 100 * (uint16_t)(local_arrs8CurrentDate[7] - '0') + 10 * (uint16_t)(local_arrs8CurrentDate[8] - '0') + (uint16_t)(local_arrs8CurrentDate[9] - '0');
    uint8_t local_u8CurrentMonth = 10 * (uint8_t)(local_arrs8CurrentDate[3] - '0') + (uint8_t)(local_arrs8CurrentDate[4] - '0');

    /*get the expiry month and year of the card*/
    uint16_t local_u16CardExpYear =  1000 * (uint16_t)(arg_arrs8ExpiryDate[3] - '0') + 100 * (uint16_t)(arg_arrs8ExpiryDate[4] - '0') + 10 * (uint16_t)(arg_arrs8ExpiryDate[5] - '0') + (uint16_t)(arg_arrs8ExpiryDate[6] - '0');
    uint8_t local_u8CardExpMonth = 10 * (uint8_t)(arg_arrs8ExpiryDate[0] - '0') + (uint8_t)(arg_arrs8ExpiryDate[1] - '0');    

    /*compare the year first and then month , if equal then the card is not expired*/
    if(local_u16CurrentYear > local_u16CardExpYear)
    {
        local_boolIsExpired = 1;
    }
    else if((local_u16CurrentYear == local_u16CardExpYear) && (local_u8CurrentMonth > local_u8CardExpMonth))
    {
        local_boolIsExpired = 1;
    }
    else
    {
        /*not required but it's here as MISRA C stated that there is must be else*/
        local_boolIsExpired = 0;
    }

    return local_boolIsExpired;
}

/*  @fn     :       -gatTransactionAmount
 *
 *  @params :       -none
 *
 *  @brief  :       -get the amount of the money that the user wants to transact to
 *
 *  @return :       -the amount of the money to be transcated
 */
float gatTransactionAmount(void)
{
    /*a buffer used to read what the user entered*/
    int8_t local_arrs8DummyBuff[20];

    /*a number that the user entered*/
    float local_f32Amount = 0;
    
    /*a flag to indicate that the user entered a correct number*/
    _Bool local_boolIsCorrect = 0;

    /*check if all entered chars are numbers*/
    do{
        fputs(BLUE "enter the amount to transact : " YELLOW, stdout);

        /*read what the user entered in a dummy buffer*/
        fgets((char*)local_arrs8DummyBuff, 20, stdin);

        /*ckech if the entered number is correct*/
        for(int i = 0; local_arrs8DummyBuff[i] != '\0'; i++)
        {
            if(local_arrs8DummyBuff[0] == '\n')
            {
                fputs(RED "you entered nothing , please enter a valid amount\n", stdout);
                break;                
            }
            else if(local_arrs8DummyBuff[i] == '\n')
            {
                local_boolIsCorrect = 1;
                /*converting the entered number to float*/
                local_arrs8DummyBuff[i] = '\0';
                local_f32Amount = atof((const char*) local_arrs8DummyBuff);

            }
            else if((local_arrs8DummyBuff[i] > '9' || local_arrs8DummyBuff[i] < '0') && local_arrs8DummyBuff[i] != '.')
            {
                fputs(RED "the entered value isn't a number\n", stdout);
                break;
            }
            else
            {
                // no operation
            }
        }

    }while(!local_boolIsCorrect);

    return local_f32Amount;
}

/*  @fn         :       -isBelowMaxAmount
 *
 *  @params[0]  :       -amount of the money to be transcated
 *
 *  @brief      :       -checks if the amount of money to be transcated is less than the limit or not
 *
 *  @return     :       - 1 : if the amount of the money to be transcated is allowed
 *                        0 : if the amount of the money to be transcated is morw than the limit
 */
_Bool isBelowMaxAmount(float arg_u32Amount)
{
    return ((global_f32MaxAmount >= arg_u32Amount) ? 1 : 0);
}

/*  @fn         :       -setMaxAmount
 *
 *  @params[0]  :       -the new max amount of money limit to be set on that terminal
 *
 *  @brief      :       -sets a new limit to the money to be transcated
 *
 * @notes       :       -only admin users are allowed to use this function
 *
 *  @return     :       - 1 : if the operation is successful
 *                        0 : if the operation failed
 */
_Bool setMaxAmount(float arg_u32MaxAmount)
{
    /*a buffer used to read what the user entered*/
    int8_t local_arrs8DummyBuff[MAX_PASS_LEN];

    /*boolean var to indicate that the entered password is correct or not*/
    _Bool local_boolIsAdmin = 0;

    /*get the user input*/
    fputs(BLUE "enter the admin password : \n" YELLOW, stdout);
    fgets((char*)local_arrs8DummyBuff, MAX_PASS_LEN, stdin);

    for(uint8_t i = 0; local_arrs8DummyBuff[i] != '\0'; i++)
    {
        if(local_arrs8DummyBuff[i] == '\n')
        {
            local_arrs8DummyBuff[i] = '\0';
            local_boolIsAdmin = 1;
        }
        else if(local_arrs8DummyBuff[i + 1] == '\0')
        {
            local_boolIsAdmin = 1;
        }
        else if(local_arrs8DummyBuff[i] != global_s8Password[i])
        {
            fputs(RED "not an admin\n", stdout);
            break;
        }
        else
        {
            // nothing to do
        }
    }

    /*then update the value in both global variable and in database*/
    if(local_boolIsAdmin == 1)
    {
        /*update the global variable*/
        global_f32MaxAmount = arg_u32MaxAmount;

        /*update the value in database*/

        /*pointer that points to the file*/
        FILE *file;

        /*open database maxAmount file for write only*/
        file = fopen(MAX_AMOUNT_DB_NAME, "w");

        /*convert the float to array character*/
        sprintf((char*)local_arrs8DummyBuff, MAX_FLOAT_PREC_STORE, arg_u32MaxAmount);

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

    return local_boolIsAdmin;
}

/*  @fn         :       -isFakePAN
 *
 *  @params[0]  :       -the PAN to be tested , 16 digits in form of array
 *
 *  @brief      :       -use a simple equation to test that the PAN isn't fake by testing the last 4 digits of the PAN which are generated by equation
 *
 *  @equation   :       -((12_MSB_number * 5) %10) * 1000 + ((12_MSB_number / 2) %10) * 100 + ((((12_MSB_number + 3) * 2) / 5) %10) * 10 + (12_MSB_number + 2) %10
 *
 *  @return     :       - 1 : if the PAN is fake
 *                        0 : if the PAN isn't fake
 */
_Bool isFakePAN(int8_t* args_arrs8PAN)
{
    /*local variable to hold the first 12 digits*/
    uint64_t local_u64NumAsInput = 0;

    /*local variable to hold the last 4 digits*/
    uint32_t local_u32NumToTest = 0;

    /*dummy variable to compare the last 4 digits using the equation with what the user entered*/
    uint32_t local_u32NumGenerated = 0;

    /*boolean variable as a flag to check if the entered PAN is fake or not*/
    _Bool local_boolIsFake = 0;

    /*get the first 12 digits*/
    for(uint8_t i = 0; i < 12; i++)
    {
        local_u64NumAsInput *= 10;
        local_u64NumAsInput += (uint64_t)(args_arrs8PAN[i] - '0');
    }

    /*get the last 4 digits*/
    for(uint8_t i = 12; i < 16; i++)
    {
        local_u32NumToTest *= 10;
        local_u32NumToTest += (uint64_t)(args_arrs8PAN[i] - '0');
    }

    /*generate the correct number*/
    local_u32NumGenerated = ((local_u64NumAsInput * 5) %10) * 1000 + ((local_u64NumAsInput / 2) %10) * 100 + ((((local_u64NumAsInput + 3) * 2) / 5) %10) * 10 + (local_u64NumAsInput + 2) %10;

    /*chechs if the generated number equal to the correct*/
    local_boolIsFake = (local_u32NumGenerated == local_u32NumToTest)? 0 : 1;

    return local_boolIsFake;
}