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
void __attribute__((signal,used)) __vector_10 (void) 
{
	/*call the call back function*/
	global_pvOutCmpCallBack();

	/*disable the flag of interrupt*/
	SET_BIT(TIFR, TIFR_OCF0_1BITS);

	/*enable the global interrupt after the end of interrupt*/
	ENABLE_GLOBAL_INT();
	
}

/*
 *	@fn			-:		-__vector_11
 *
 *	@brief		-:		-ISR of Timer/Counter0 Compare Match
 *
 */
void __attribute__((signal,used)) __vector_11(void) 
{
	/*call the call back function*/
	global_pvOverFlowCallBack();

	/*disable the flag of interrupt*/
	SET_BIT(TIFR, TIFR_TOV0_1BITS);

	/*enable the global interrupt after the end of interrupt*/
	ENABLE_GLOBAL_INT();
	
}

static void dummyCallBackFunc()
{
	// increase the number of overflows
	global_countVar++;

	// reset the value of the counter
	TCNT0 = global_u8InitValForTim;

	// call the 
	if (global_countVar == global_ValueToReachCount)
	{
		/*reset the global variable*/
		global_countVar = 0;

		/*call the call back function*/
		global_pvCallBackDelay();
	}
}

/*
 *	@fn			-:		-calculatInitValueForTimer0
 *
 *	@params[0]	-:		-a number in milliseconds to delay for 
 *
 *	@brief		-:		-calculate the initial value needed for timer0 to be inserted into the timer
 *
 *	@return		-:		-the initial value to be in the timer0
 */
static uint8_t calculatInitValueForTimer0(uint32_t args_u32TimeInMilliSeconds, uint16_t args_u8Prescalar)
{
	/*local variable for time in seconds*/
	double volatile local_f64TimerInSeconds = args_u32TimeInMilliSeconds / 1000.0;

	/*local variable that will contain the value for init timer*/
	uint8_t volatile local_u8TimerInit = 0;

	/*local variable that will contain the time for one tick*/
	double volatile local_f64Ttick;


	/*local variable that will contain the time for max delay*/
	double volatile local_f64Tmaxdelay;

	/*get the tick timer*/
	local_f64Ttick = args_u8Prescalar / 1000000.0;

	/*get the max delay*/
	local_f64Tmaxdelay = 256 * local_f64Ttick;

	/*see which init time to be used*/
	if (local_f64TimerInSeconds == (uint32_t) local_f64Tmaxdelay)
	{
		/*only one overflow needed*/
		global_ValueToReachCount = 1;
		/*begin counting from the start*/
		local_u8TimerInit = 0;
	}
	else if (local_f64TimerInSeconds < (uint32_t) local_f64Tmaxdelay)
	{
		/*only one overflow needed*/
		global_ValueToReachCount = 1;
		/*begin counting from the start*/
		local_u8TimerInit = (uint8_t)((local_f64Tmaxdelay - local_f64TimerInSeconds) / local_f64Ttick);
	}
	else if (local_f64TimerInSeconds > (uint32_t) local_f64Tmaxdelay)
	{
		/*many overflow needed*/
		global_ValueToReachCount = ((local_f64TimerInSeconds / local_f64Tmaxdelay) == ((uint32_t)local_f64TimerInSeconds / (uint32_t)local_f64Tmaxdelay)) ? (uint32_t)(local_f64TimerInSeconds / local_f64Tmaxdelay) : (uint32_t)(local_f64TimerInSeconds / local_f64Tmaxdelay) + 1;
		
		/*begin counting from the start*/
		local_u8TimerInit = 256 -  (uint8_t)( (local_f64TimerInSeconds / local_f64Ttick) / global_ValueToReachCount);
	}

	/*return the calculated value*/
	return local_u8TimerInit;
}

/*
 *	@fn			-:		-timer0_delay
 *
 *	@params[0]	-:		-a number in milliseconds to delay for 
 *
 *	@params[1]	-:		-a call back function to be called after the end of the delay
 *
 *	@brief		-:		-delay a specific amount of time
 *
 *	@return		-:		-none
 */
void timer0_InterruptEvery(uint32_t args_u32TimeInMilliSeconds,  void (*args_pvCallBackFunc)(void))
{
	/*assign the call back function*/
	global_pvCallBackDelay = args_pvCallBackFunc;

	/*assign the dummy call back function*/
	global_pvOverFlowCallBack = dummyCallBackFunc;

	/*initialize the timer with normal mode*/
	timer0_init(0, TIM0_MODE_NORMAL, TIM0_COM_NORM_NORMAL, dummyCallBackFunc, dummyCallBackFunc);

	/*calculate all required parameters*/
	global_u8InitValForTim = calculatInitValueForTimer0(args_u32TimeInMilliSeconds, (uint16_t)1024);

	/*init TCNT0*/
	TCNT0 = global_u8InitValForTim;

	/*start the timer*/
	timer0_start(TIM0_CLK_SRC_PRES_1024);	

	/*enable the global interrupt*/
	ENABLE_GLOBAL_INT();
	
}