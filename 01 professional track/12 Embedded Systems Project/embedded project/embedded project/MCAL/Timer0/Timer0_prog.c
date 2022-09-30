/*
 * Timer0_prog.c
 *
 * Created: 25/09/2022 6:59:16 pm
 *  Author: abdo
 */ 

 #include "Timer0_config.h"
#include "Timer0_header.h"
#include "Timer0_private.h"
#include "Timer0_reg.h"

#include "../../utils/math_btt.h"
#include "../../utils/standard_int.h"
#include "../atmega32_uPU/atmega32_uPU_header.h"


/*
 *	@fn			-:		-timer0_init
 *
 *	@params[0]	-:		-symbol representing the timer mode, options are in header file , refer to @OPTIONS_TOMER0_MODES
 *
 *	@params[1]	-:		-symbol representing the output pin mode when dealing non-normal mode, options are in header file , refer to @OPTIONS_OUTPUT_MODE_NON_PWM or @OPTIONS_OUTPUT_MODE_FAST_PWM or @OPTIONS_OUTPUT_MODE_PHASE_PWM
 *
 *	@params[2]	-:		-a call back function when there is overflow happens
 *
 *	@params[3]	-:		-a call back function when there is output compare match happens
 *
 *	@brief		-:		-initialize a a timer with a specific mode
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_TIMER0_t timer0_init(uint8_t args_timer0_initValue, uint8_t args_u8timer0_mode, uint8_t args_u8timer0_outpin_mode, void (*args_pvCallBackFuncOverflow)(void), void (*args_pvCallBackOutCmp)(void))
{
	/*local variable to indicate the error state*/
	ES_TIMER0_t local_errorState_t = TIM0_OK;

	/*check for the validity of the input*/
	if((args_u8timer0_mode > TIM0_MODE_PHASE_PWM) || (args_u8timer0_mode == TIM0_MODE_NORMAL && args_u8timer0_outpin_mode > TIM0_COM_NORM_SET) || (args_u8timer0_mode < TIM0_MODE_PHASE_PWM && args_u8timer0_outpin_mode == 1))
	{
		local_errorState_t = TIM0_INVALID_IN;
	}
	else
	{
		/*setup the mode for the timer*/
		ASSIGN_BIT(TCCR0, TCCR0_WGM00_1BITS, GET_BIT(args_u8timer0_mode, 0));
		ASSIGN_BIT(TCCR0, TCCR0_WGM01_1BITS, GET_BIT(args_u8timer0_mode, 1));

		/*set up the callback function*/
		global_pvOutCmpCallBack = args_pvCallBackOutCmp;
		global_pvOverFlowCallBack = args_pvCallBackFuncOverflow;

		/*set up the mode of the output pin*/
		ASSIGN_RANGE(TCCR0, TCCR0_COM0_2BITS, TCCR0_COM0_2BITS + 1, args_u8timer0_outpin_mode);

		/*setup the timer initial value*/
		TCNT0 = args_timer0_initValue;

		/*enable the interrupt flag for overflow*/
		SET_BIT(TIMSK, TIMSK_TOIE0_1BITS);

	}

	/*return that local error*/
	return local_errorState_t;
}

/*
 *	@fn			-:		-timer0_SetCompareReg
 *
 *	@params[0]	-:		-a number to compare the timer0 value with
 *
 *	@brief		-:		-sets the output compare register with the passed value
 *
 *	@return		-:		-none
 */
ES_TIMER0_t timer0_SetCompareReg(uint8_t args_u8tOCRVal)
{
	/*setup the OCR register*/
	OCR0 = args_u8tOCRVal;

	/*enable the interrupt flag for output compare*/
	SET_BIT(TIMSK, TIMSK_OCIE0_1BITS);

	return TIM0_OK;
}

ES_TIMER0_t timer0_start(uint8_t args_u8ClockSelect)
{
	/*local variable to indicate the error state*/
	ES_TIMER0_t local_errorState_t = TIM0_OK;	

	/*check for the validity of the input*/
	if(args_u8ClockSelect > TIM0_CLK_SRC_EXT_RISE)
	{
		local_errorState_t = TIM0_INVALID_IN;	
	}
	else
	{
		/*enable the clock*/
		ASSIGN_RANGE(TCCR0, TCCR0_CS0_3BITS, TCCR0_CS0_3BITS + 2, args_u8ClockSelect);
	}	
	

	/*return that local error*/
	return local_errorState_t;
}

ES_TIMER0_t timer0_stop()
{
	/*stop the timer*/
	CLR_RANGE(TCCR0, TCCR0_CS0_3BITS, TCCR0_CS0_3BITS + 2);

	return TIM0_OK;
}

/*
 *	@fn			-:		-__vector_10
 *
 *	@brief		-:		-ISR of Timer/Counter1 Overflow
 *
 */
void __attribute__ ((signal,used)) __vector_10 (void) 
{
	/*call the call back function*/
	global_pvOverFlowCallBack();

	/*disable the flag of interrupt*/
	SET_BIT(TIFR, TIFR_TOV0_1BITS);

	/*enable the global interrupt after the end of interrupt*/
	ENABLE_GLOBAL_INT();
	
}

/*
 *	@fn			-:		-__vector_11
 *
 *	@brief		-:		-ISR of Timer/Counter0 Compare Match
 *
 */
void __attribute__ ((signal,used)) __vector_11(void) 
{
	/*call the call back function*/
	global_pvOutCmpCallBack();

	/*disable the flag of interrupt*/
	SET_BIT(TIFR, TIFR_OCF0_1BITS);	

	/*enable the global interrupt after the end of interrupt*/
	ENABLE_GLOBAL_INT();
	
}