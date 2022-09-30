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



#endif /* TIMER0_PRIVATE_H_ */