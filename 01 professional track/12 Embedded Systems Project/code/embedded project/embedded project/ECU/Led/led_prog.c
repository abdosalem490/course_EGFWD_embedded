/*
 * led_prog.c
 *
 * Created: 28/09/2022 6:06:38 am
 *  Author: abdo
 */ 


 #include "led_config.h"
 #include "led_header.h"
 #include "led_private.h"



/*
 *	@fn			-:		-led_init
 *
 *	@params[0]	-:		-symbol representing the port name , options are in GPIO header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in GPIO  header file , refer to @PINS_NAMES
 *
 *	@brief		-:		-initializes a pin to be output
 *
 *	@return		-:		-return error state indicating the validity of the input
 */
 ES_LED_t led_init(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber)
 {
	/*initialize the pin to be output*/
	if(GPIO_PinInit(args_u8PortNumber, args_u8PinNumber, GPIO_MODE_OUT) != GPIO_OK)
		return LED_INVALID_IN;
	else
		return LED_OK;


 }

 /*
 *	@fn			-:		-led_init
 *
 *	@params[0]	-:		-symbol representing the port name , options are in GPIO header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in GPIO  header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-symbol representing the connection of the LED, options are in  header file , refer to @OPTIONS_LED_TYPES
 *
 *	@params[3]	-:		-symbol representing the state of the LED, options are in  header file , refer to @OPTIONS_LED_STATES
 *
 *	@brief		-:		-writes a value to the pin
 *
 *	@return		-:		-return error state indicating the validity of the input
 */
 ES_LED_t led_Write(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, uint8_t args_u8LedType, uint8_t args_u8Value)
 {
 	if(args_u8LedType > LED_TYPE_ANODE || args_u8Value > LED_STATE_OFF)
 	{
	 	return LED_INVALID_IN;
 	}
 	else
 	{
	 	if(args_u8LedType == LED_TYPE_ANODE && args_u8Value == LED_STATE_ON)
	 	{
		 	if(GPIO_PinsWrite(args_u8PortNumber, args_u8PinNumber, HIGH) != GPIO_OK)
		 		return LED_INVALID_IN;
		 	else
		 		return LED_OK;

	 	}
	 	else if(args_u8LedType == LED_TYPE_ANODE && args_u8Value == LED_STATE_OFF)
	 	{
		 	if(GPIO_PinsWrite(args_u8PortNumber, args_u8PinNumber, LOW) != GPIO_OK)
		 		return LED_INVALID_IN;
		 	else
		 		return LED_OK;

	 	}
	 	else if(args_u8LedType == LED_TYPE_CATHODE && args_u8Value == LED_STATE_ON)
	 	{
		 	if(GPIO_PinsWrite(args_u8PortNumber, args_u8PinNumber, LOW) != GPIO_OK)
		 		return LED_INVALID_IN;
		 	else
		 		return LED_OK;

	 	}
	 	else if(args_u8LedType == LED_TYPE_CATHODE && args_u8Value == LED_STATE_OFF)
	 	{
		 	if(GPIO_PinsWrite(args_u8PortNumber, args_u8PinNumber, HIGH) != GPIO_OK)
		 		return LED_INVALID_IN;
		 	else
		 		return LED_OK;

	 	}

 	}
	return LED_INVALID_IN;

 }

 /*
 *	@fn			-:		-led_init
 *
 *	@params[0]	-:		-symbol representing the port name , options are in GPIO header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in GPIO  header file , refer to @PINS_NAMES
 *
 *	@brief		-:		-writes a value to the pin
 *
 *	@return		-:		-return error state indicating the validity of the input
 */
 ES_LED_t led_Toggle(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber)
 {
	if(GPIO_PinsToggle(args_u8PortNumber, args_u8PinNumber) != GPIO_OK)
		return LED_INVALID_IN;
	else
		return LED_OK;

 }
