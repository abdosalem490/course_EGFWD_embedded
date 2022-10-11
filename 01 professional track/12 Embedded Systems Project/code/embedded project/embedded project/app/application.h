/*
 * application.h
 *
 * Created: 01/10/2022 10:00:30 pm
 *  Author: abdo
 */ 


#ifndef APPLICATION_H_
#define APPLICATION_H_

void padestrianModeLedPateern(void);

void appSetup();

void __attribute__((noreturn)) appStart();


 void timerOverFlowCallBack(void);

 void INT0CallBack(void);

 #define APP_MODE_NORM		0
 #define APP_MODE_PAD		1

 #define CAR_GREEN_LED		0
 #define CAR_YELLOW_LED		1
 #define CAR_RED_LED		2
 #define PAD_GREEN_LED		3
 #define PAD_YELLOW_LED		4
 #define PAD_RED_LED		5



#endif /* APPLICATION_H_ */