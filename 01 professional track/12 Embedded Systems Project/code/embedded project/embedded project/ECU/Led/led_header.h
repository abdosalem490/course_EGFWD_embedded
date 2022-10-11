/*
 * led_header.h
 *
 * Created: 28/09/2022 6:06:03 am
 *  Author: abdo
 */ 

 #include "../../utils/standard_int.h"
 #include "../../utils/math_btt.h"
 #include "../../MCAL/GPIO/GPIO_header.h"

#ifndef LED_HEADER_H_
#define LED_HEADER_H_

//types of led connections : @OPTIONS_LED_TYPES
#define LED_TYPE_CATHODE			0
#define LED_TYPE_ANODE				1

//types of led status : @OPTIONS_LED_STATES
#define LED_STATE_ON				0
#define LED_STATE_OFF				1

// struct to hold the information about the led
typedef struct Led_t{
	uint8_t args_u8PortNumber;
	uint8_t args_u8PinNumber;
	uint8_t args_u8LedType;
	uint8_t args_u8Value;
}Led_t;


//error states related to led
typedef enum ES_LED_t{
	LED_OK			= 0,
	LED_INVALID_IN	= 1,
}ES_LED_t;


/************************************************************************/
/* public functions                                                     */
/************************************************************************/
ES_LED_t led_init(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber);
ES_LED_t led_Write(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, uint8_t args_u8LedType, uint8_t args_u8Value);
ES_LED_t led_Toggle(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber);


#endif /* LED_HEADER_H_ */