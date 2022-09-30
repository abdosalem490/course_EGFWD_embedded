/*
 * GPIO_private.h
 *
 * Created: 24/09/2022 12:33:13 am
 *  Author: abdo
 */ 


#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_


/*global variables used to call back user defined function when interrupt happens*/
void (*global_pvCallBackINT0)(void);
void (*global_pvCallBackINT1)(void);
void (*global_pvCallBackINT2)(void);

void __vector_1(void) __attribute__((signal, used));
void __vector_2(void) __attribute__((signal, used));
void __vector_3(void) __attribute__((signal, used));

#endif /* GPIO_PRIVATE_H_ */