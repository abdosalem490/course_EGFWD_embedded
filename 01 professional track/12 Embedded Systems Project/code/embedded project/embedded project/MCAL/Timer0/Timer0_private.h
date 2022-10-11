/*
 * Timer0_private.h
 *
 * Created: 25/09/2022 6:58:58 pm
 *  Author: abdo
 */ 


#ifndef TIMER0_PRIVATE_H_
#define TIMER0_PRIVATE_H_

/*call back function when there is an overflow flag*/
void (*global_pvOverFlowCallBack)(void);

/*call back function when there is an output compare flag*/
void (*global_pvOutCmpCallBack)(void);

/*dummy call back function used in timer delay*/
static void dummyCallBackFunc();

/*a call back function call whenever the delay is completer*/
void (*global_pvCallBackDelay)(void);

/*count variable used to count the number of overflows*/
static volatile uint32_t global_countVar = 0;
static volatile uint32_t global_ValueToReachCount = 0;
static volatile uint8_t global_u8InitValForTim = 0;

/*calculate the initial value for the timer*/
static uint8_t calculatInitValueForTimer0(uint32_t args_u32TimeInMilliSeconds, uint16_t args_u8Prescalar);


#endif /* TIMER0_PRIVATE_H_ */