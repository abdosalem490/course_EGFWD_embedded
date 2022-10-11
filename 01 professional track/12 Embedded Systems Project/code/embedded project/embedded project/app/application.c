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
 #include "../MCAL/SleepModes/SleepModes_header.h"

/*global variables*/
Led_t volatile carsGreen_t;
Led_t volatile carsYellow_t;
Led_t volatile carsRed_t;

Led_t volatile peopleGreen_t;
Led_t volatile peopleYellow_t;
Led_t volatile peopleRed_t;

Button_t volatile userButton_t;

Timer0_t volatile tim0_t;

/*seconds timer*/
uint32_t volatile global_u32Seconds = 1;

uint8_t volatile global_u8Mode = APP_MODE_NORM;

uint8_t volatile global_u8ActiveLed = CAR_GREEN_LED;

uint8_t volatile global_u8CheckForLongPress = 0;

uint8_t volatile global_u8ArrTurns[4] = {CAR_GREEN_LED, CAR_YELLOW_LED, CAR_RED_LED, CAR_YELLOW_LED};

/*variable used to go to the next element in the arr*/
static volatile uint32_t local_u32ArrIndex = 0;

void checkIfToSwitchToNextLed(void)
{


	/*switch to the next led if 5 seconds passed*/
	if (global_u32Seconds % 6 == 0)
	{
		/*turn of the current led*/
		switch(global_u8ActiveLed)
		{
			case CAR_GREEN_LED:
				led_Write(carsGreen_t.args_u8PortNumber, carsGreen_t.args_u8PinNumber, carsGreen_t.args_u8LedType, LED_STATE_OFF);
				break;

			case CAR_YELLOW_LED:
				led_Write(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber, carsYellow_t.args_u8LedType, LED_STATE_OFF);
				break;
				
			case CAR_RED_LED:
				led_Write(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber, carsRed_t.args_u8LedType, LED_STATE_OFF);
				break;			
				
			default:
				break;				
		}
		
		/*go to the next led*/
		local_u32ArrIndex = (local_u32ArrIndex+1) % 4;

		global_u8ActiveLed = global_u8ArrTurns[local_u32ArrIndex];

		/*turn on the next led*/
		switch(global_u8ActiveLed)
		{
			case CAR_GREEN_LED:
				led_Write(carsGreen_t.args_u8PortNumber, carsGreen_t.args_u8PinNumber, carsGreen_t.args_u8LedType, LED_STATE_ON);
				led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_ON);
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_OFF);
				break;

			case CAR_YELLOW_LED:
				led_Write(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber, carsYellow_t.args_u8LedType, LED_STATE_ON);
				break;
					
			case CAR_RED_LED:
				led_Write(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber, carsRed_t.args_u8LedType, LED_STATE_ON);
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_ON);
				led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_OFF);
				break;
					
			default:
				break;
		}
	}
}


void padestrianModeLedPateern(void)
{
	static uint8_t local_u8Count = 0;

	/*increase the time by 1*/
	local_u8Count++;

	/*check the active led*/
	switch(global_u8ActiveLed)
	{
		case CAR_GREEN_LED:
		case CAR_YELLOW_LED:
		{	

			/*turn on the pedestrian red led*/
			if(local_u8Count == 0)
				led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_ON);
			/*turn off all leds*/
			else if(local_u8Count == 5)
			{
				led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_OFF);
				led_Write(carsGreen_t.args_u8PortNumber, carsGreen_t.args_u8PinNumber, carsGreen_t.args_u8LedType, LED_STATE_OFF);
			}
			/*blink the yellow leds*/
			else if(local_u8Count >= 5 && local_u8Count < 10)
			{
				led_Toggle(peopleYellow_t.args_u8PortNumber, peopleYellow_t.args_u8PinNumber);
				led_Toggle(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber);
			}
			/*turn off all yellow leds*/
			else if(local_u8Count == 10)
			{
				led_Write(peopleYellow_t.args_u8PortNumber, peopleYellow_t.args_u8PinNumber, peopleYellow_t.args_u8LedType, LED_STATE_OFF);
				led_Write(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber, carsYellow_t.args_u8LedType, LED_STATE_OFF);
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_ON);
				led_Write(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber, carsRed_t.args_u8LedType, LED_STATE_ON);
			}
			/*turn of red led*/
			else if(local_u8Count == 15)
			{
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_OFF);
				led_Write(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber, carsRed_t.args_u8LedType, LED_STATE_OFF);
			}
			/*toggle the yellow leds of the car*/
			else if(local_u8Count >= 15 && local_u8Count < 20)
			{
				led_Toggle(peopleYellow_t.args_u8PortNumber, peopleYellow_t.args_u8PinNumber);
				led_Toggle(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber);
			}
			/*go back to normal mode*/
			else if(local_u8Count == 20)
			{
				global_u8ActiveLed = CAR_GREEN_LED;
				local_u32ArrIndex = 0;

				/*go back to normal mode*/
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_OFF);
				led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_ON);
				led_Write(carsGreen_t.args_u8PortNumber, carsGreen_t.args_u8PinNumber, carsGreen_t.args_u8LedType, LED_STATE_ON);
	
				global_u8Mode = APP_MODE_NORM;

				local_u8Count = 0;
			}
			break;
		}
		case CAR_RED_LED:
		{	
			/*the pedestrian's Green LED and the cars' Red LEDs will be on for five seconds*/
			if(local_u8Count == 1)
			{	
				led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_OFF);
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_ON);
				led_Write(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber, carsRed_t.args_u8LedType, LED_STATE_ON);
			}
			else if(local_u8Count == 5)
			{
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_OFF);
				led_Write(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber, carsRed_t.args_u8LedType, LED_STATE_OFF);
			}
			/*toggle the yellow leds of the car*/
			else if(local_u8Count >= 5 && local_u8Count < 10)
			{
				led_Toggle(peopleYellow_t.args_u8PortNumber, peopleYellow_t.args_u8PinNumber);
				led_Toggle(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber);
			}
			/*go back to normal mode*/
			else if(local_u8Count == 10)
			{
				global_u8ActiveLed = CAR_GREEN_LED;
				local_u32ArrIndex = 0;

				/*go back to normal mode*/
				led_Write(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber, peopleGreen_t.args_u8LedType, LED_STATE_OFF);
				led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_ON);
				led_Write(carsGreen_t.args_u8PortNumber, carsGreen_t.args_u8PinNumber, carsGreen_t.args_u8LedType, LED_STATE_ON);
				
				global_u8Mode = APP_MODE_NORM;

				local_u8Count = 0;
			}

			//led_Write(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber, carsRed_t.args_u8LedType, LED_STATE_ON);
			break;
		}
				
		default:
			/*go back to normal mode*/
			global_u8Mode = APP_MODE_NORM;
			break;
	}	


}

 void timerOverFlowCallBack(void)
 {
	/*increment number of seconds*/
	global_u32Seconds++;

	/*check if we are in normal mode or not*/
	if (global_u8Mode == APP_MODE_NORM)
	{
		/*the yellow led will always blink*/
		if (global_u8ActiveLed == CAR_YELLOW_LED)
		{
			led_Toggle(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber);
		}

		/*check if we to switch to the next led or not*/
		checkIfToSwitchToNextLed();
	
	}
	/*check if we are in pedestrian mode or not*/
	else if(global_u8Mode == APP_MODE_PAD)
	{
		/*check if the button is still pressed then it's a long press*/
		if (global_u8CheckForLongPress == 0)
		{
			button_read(userButton_t.args_u8PortNumber, userButton_t.args_u8PinNumber, (uint8_t*)&userButton_t.args_u8Input, userButton_t.args_u8ButtonType);
			global_u8CheckForLongPress = 1;
		}

		if(userButton_t.args_u8Input == BUTTON_STATE_PRESSED)
		{
			/*go back to normal mode*/
			global_u8Mode = APP_MODE_NORM;

			global_u8CheckForLongPress = 0;

		}
		else
		{
			/*switch to pedestrian mode*/
			padestrianModeLedPateern();
		}
	}
	/*go back to normal mode*/
	else
	{
		global_u8Mode = APP_MODE_NORM;
	}
 }

void INT0CallBack(void)
{
	/*software delay for button debouncing*/
	for(int i = 0; i < 5000; i++);

	/*if user is in pedestrian mode then we switch to pedestrian mode*/
	if(global_u8Mode == APP_MODE_NORM && global_u8ActiveLed != CAR_RED_LED)
	{
		global_u8Mode = APP_MODE_PAD;
	}

}

 void appSetup()
 {
	/*setup Leds Type*/
	carsGreen_t.args_u8LedType = LED_TYPE_ANODE;
	carsYellow_t.args_u8LedType = LED_TYPE_ANODE;
	carsRed_t.args_u8LedType = LED_TYPE_ANODE;

	peopleGreen_t.args_u8LedType = LED_TYPE_ANODE;
	peopleYellow_t.args_u8LedType = LED_TYPE_ANODE;
	peopleRed_t.args_u8LedType = LED_TYPE_ANODE;

	/*setup Leds port Number*/
	carsGreen_t.args_u8PortNumber = GPIO_PORTA;
	carsYellow_t.args_u8PortNumber = GPIO_PORTA;
	carsRed_t.args_u8PortNumber = GPIO_PORTA;

	peopleGreen_t.args_u8PortNumber = GPIO_PORTB;
	peopleYellow_t.args_u8PortNumber = GPIO_PORTB;
	peopleRed_t.args_u8PortNumber = GPIO_PORTB;

	/*setup Leds PinNumbers*/
	carsGreen_t.args_u8PinNumber = GPIO_PIN0;
	carsYellow_t.args_u8PinNumber = GPIO_PIN1;
	carsRed_t.args_u8PinNumber = GPIO_PIN2;

	peopleGreen_t.args_u8PinNumber = GPIO_PIN0;
	peopleYellow_t.args_u8PinNumber = GPIO_PIN1;
	peopleRed_t.args_u8PinNumber = GPIO_PIN2;

	/*setup Leds init Values*/
	carsGreen_t.args_u8Value = LED_STATE_OFF;
	carsYellow_t.args_u8Value = LED_STATE_OFF;
	carsRed_t.args_u8Value = LED_STATE_OFF;

	peopleGreen_t.args_u8Value = LED_STATE_OFF;
	peopleYellow_t.args_u8Value = LED_STATE_OFF;
	peopleRed_t.args_u8Value = LED_STATE_OFF;

	/*setup button data*/
	userButton_t.args_u8PortNumber = GPIO_PORTD;
	userButton_t.args_u8PinNumber = GPIO_PIN2;
	userButton_t.args_pvCallBack = INT0CallBack;
	userButton_t.args_u8ButtonType = BUTTON_TYPE_PULL_UP;
	userButton_t.args_u8InternOrExternalRes = BUTTON_INTERNAL_PULL_UP;
	userButton_t.args_u8IntMode = GPIO_INT_MODE_FALL_EDGE;
	userButton_t.args_u8IntNum = GPIO_INT0;


	/*Setup timer0 initial data*/
	tim0_t.args_u32TimeInMilliSeconds = 1000;
	tim0_t.args_pvCallBackFunc = timerOverFlowCallBack;


	/*initialize the all the Leds */
	led_init(carsGreen_t.args_u8PortNumber, carsGreen_t.args_u8PinNumber);
	led_init(carsYellow_t.args_u8PortNumber, carsYellow_t.args_u8PinNumber);
	led_init(carsRed_t.args_u8PortNumber, carsRed_t.args_u8PinNumber);
	led_init(peopleGreen_t.args_u8PortNumber, peopleGreen_t.args_u8PinNumber);
	led_init(peopleYellow_t.args_u8PortNumber, peopleYellow_t.args_u8PinNumber);
	led_init(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber);

	/*initialize the button*/
	button_init_IT(userButton_t.args_u8IntNum, userButton_t.args_u8IntMode, userButton_t.args_pvCallBack, userButton_t.args_u8InternOrExternalRes);

	/*turn on the green led*/
	led_Write(carsGreen_t.args_u8PortNumber, carsGreen_t.args_u8PinNumber, carsGreen_t.args_u8LedType, LED_STATE_ON);
	/*people red led*/
	led_Write(peopleRed_t.args_u8PortNumber, peopleRed_t.args_u8PinNumber, peopleRed_t.args_u8LedType, LED_STATE_ON);

	/*setup the timer interrupt to be called every 1 second*/
	timer0_InterruptEvery(tim0_t.args_u32TimeInMilliSeconds, tim0_t.args_pvCallBackFunc);


 }

 void __attribute__((noreturn)) appStart()
 {
	/*entering the sleep mode*/
	while(1){
		enterSleepMode(SM_IDLE);
	};
 }