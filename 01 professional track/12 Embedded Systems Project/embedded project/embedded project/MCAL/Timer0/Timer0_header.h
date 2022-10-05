/*
 * Timer0_header.h
 *
 * Created: 25/09/2022 6:58:04 pm
 *  Author: abdo
 */ 

 #include "../../utils/standard_int.h"

#ifndef TIMER0_HEADER_H_
#define TIMER0_HEADER_H_


/*timer0 modes : @OPTIONS_TOMER0_MODES*/
#define TIM0_MODE_NORMAL			0
#define TIM0_MODE_CTC				1
#define TIM0_MODE_FAST_PWM			2
#define TIM0_MODE_PHASE_PWM			3

/*output compare pin mode options for non PWM mode : @OPTIONS_OUTPUT_MODE_NON_PWM*/
#define TIM0_COM_NORM_NORMAL		0			/*Normal port operation, OC0 disconnected*/
#define TIM0_COM_NORM_TOGGLE		1			/*Toggle OC0 on compare match*/
#define TIM0_COM_NORM_CLR			2			/*Clear OC0 on compare match*/
#define TIM0_COM_NORM_SET			3			/*Set OC0 on compare match*/

/*output compare pin mode options for fast PWM mode : @OPTIONS_OUTPUT_MODE_FAST_PWM*/
#define TIM0_COM_FPWM_NORM			0			/*Normal port operation, OC0 disconnected.*/
#define TIM0_COM_FPWM_CLR			2			/*Clear OC0 on compare match, set OC0 at BOTTOM,*/
#define TIM0_COM_FPWM_SET			3			/*Set OC0 on compare match, clear OC0 at BOTTOM*/

/*output compare pin mode options for phase correct PWM mode : @OPTIONS_OUTPUT_MODE_PHASE_PWM*/
#define TIM0_COM_PPWM_NORM			0			/*Normal port operation, OC0 disconnected.*/
#define TIM0_COM_PPWM_CLR			2			/*Clear OC0 on compare match when up-counting. Set OC0 on compare match when down counting.*/
#define TIM0_COM_PPWM_SET 			3			/*Set OC0 on compare match when up-counting. Clear OC0 on compare match when down counting*/

/*clock select option : @OPTION_CLOCK_SELECT*/
#define TIM0_CLK_SRC_STOP			0			/*No clock source (Timer/Counter stopped)*/
#define TIM0_CLK_SRC_PRES_NO		1			/*clk(I/O)/(No prescaling)*/
#define TIM0_CLK_SRC_PRES_8			2			/*clk(I/O)/8 (From prescaler)*/
#define TIM0_CLK_SRC_PRES_64		3			/*clk(I/O)/64 (From prescaler)*/
#define TIM0_CLK_SRC_PRES_256		4			/*clk(I/O)/256 (From prescaler)*/
#define TIM0_CLK_SRC_PRES_1024		5			/*clk(I/O)/1024 (From prescaler)*/
#define TIM0_CLK_SRC_EXT_FALL		6			/*External clock source on T0 pin. Clock on falling edge.*/
#define TIM0_CLK_SRC_EXT_RISE		7			/*External clock source on T0 pin. Clock on rising edge.*/

//error state enum to indicate the validity of the input
typedef enum ES_TIMER0_t{
	TIM0_OK				=	0,
	TIM0_INVALID_IN		=	1,
}ES_TIMER0_t;


/*driver functions*/
ES_TIMER0_t timer0_init(uint8_t args_timer0_initValue, uint8_t args_u8timer0_mode, uint8_t args_u8timer0_outpin_mode, void (*args_pvCallBackFuncOverflow)(void), void (*args_pvCallBackOutCmp)(void));
ES_TIMER0_t timer0_SetCompareReg(uint8_t args_u8tOCRVal);
ES_TIMER0_t timer0_start(uint8_t args_u8ClockSelect);
ES_TIMER0_t timer0_stop();
void timer0_InterruptEvery(uint32_t args_u32TimeInMilliSeconds,  void (*args_pvCallBackFunc)(void));


#endif /* TIMER0_HEADER_H_ */