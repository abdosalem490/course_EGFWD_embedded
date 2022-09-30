/*
 * embedded project.c
 *
 * Created: 20/09/2022 6:28:01 pm
 * Author : abdo
 */ 

#include "MCAL/atmega32_uPU/atmega32_uPU_header.h"
#include "utils/math_btt.h"


int main(void)
{
	volatile char s = 0;

	ASSIGN_RANGE(s, 4, 6, 7);

	SAVE_SREG_IN_VAR(s);

	RESTORE_SREG_FROM_VAR(s);

    while (1) 
    {
    }
}


