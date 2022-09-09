#include "terminal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/*************************************************** global variables ****************************************************/

/*these variable are meant to be static to be protected from any modification*/
static uint32_t global_u32MaxAmount = 0;
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
    global_u32MaxAmount = atoi((const char*)local_s8ArrDummy);

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