/*
 * application.c
 *
 * Created: 01/10/2022 10:00:22 pm
 *  Author: abdo
 */ 

 #include "application.h"
 #include "../ECU/Led/led_header.h"
 #include "../ECU/Button/button_header.h"
 #include "../MCAL/Timer0/Timer0_header.h"

 void func()
 {
	led_Toggle(GPIO_PORTD, GPIO_PIN7);
 }

 void appSetup()
 {
	led_init(GPIO_PORTD, GPIO_PIN7);

	timer0_InterruptEvery(1000, func);


	
	//button_init_IT(GPIO_INT0, GPIO_INT_MODE_FALL_EDGE, func, BUTTON_INTERNAL_PULL_UP);

 }

 void __attribute__((noreturn)) appStart()
 {

	while(1);
 }