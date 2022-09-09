#include "../Server_Module/server.h"
#include "../Terminal_Module/terminal.h"
#include "../Card_Module/card.h"
#include <stdio.h>



int main()
{
    //printf("your name is %s\n",(char*)getCardHolderName());
    //printf("your expiry date is %s\n",(char*)getCardExpiryDate());
    //printf("your PAN is %s\n",(char*)getCardPAN());

    getTransactionDate();
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