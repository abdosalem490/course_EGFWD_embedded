/*
 * button_prog.c
 *
 * Created: 28/09/2022 6:07:38 am
 *  Author: abdo
 */ 

 #include "button_config.h"
 #include "button_header.h"
 #include "button_private.h"

 #include "../../MCAL/GPIO/GPIO_header.h"

 /*
 *	@fn			-:		-button_init_Polling
 *
 *	@params[0]	-:		-symbol representing the port name , options are in GPIO header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in GPIO  header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-symbol representing whether external or internal resistor to be used , options are in  header file , refer to @PINS_NAMES
 *
 *	@brief		-:		-initializes a pin to 
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_BUTTON_t button_init_Polling(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, uint8_t args_u8InternOrExternalRes)
{	
	/*local variable to indicate the validity of the input*/
	ES_BUTTON_t local_errorState  = BUTTON_OK;

	// validate the input of args_u8InternOrExternalRes
	if (args_u8InternOrExternalRes > BUTTON_EXTERNAL_PULL_UP)
	{
		local_errorState = BUTTON_INVALID_IN; 
	}
	else
	{	
		/*external pull resistor*/
		if(args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_UP)
		{
			if(GPIO_PinInit(args_u8PortNumber, args_u8PinNumber, GPIO_MODE_IN) != GPIO_OK)
				local_errorState = BUTTON_INVALID_IN; 
		}
		else if(args_u8InternOrExternalRes == BUTTON_INTERNAL_PULL_UP)
		{
			if(GPIO_PinInit(args_u8PortNumber, args_u8PinNumber, GPIO_MODE_IN_PULLUP) != GPIO_OK)
				local_errorState = BUTTON_INVALID_IN;

		}
	}

	/*return the error state*/
	return local_errorState;
}


 /*
 *	@fn			-:		-button_init_IT
 *
 *	@params[0]	-:		-symbol representing the port name , options are in GPIO header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in GPIO  header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-a call back function to be called when there is external interrupt
 *
 *	@params[3]	-:		-symbol representing whether external or internal resistor to be used , options are in  header file , refer to @PINS_NAMES
 *
 *	@brief		-:		-initializes a pin to 
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_BUTTON_t button_init_IT(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, void (*args_pvCallBack) (void), uint8_t args_u8InternOrExternalRes)
{

}

ES_BUTTON_t button_read(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, uint8_t *args_u8Input)
{

}

ES_BUTTON_t button_deinit_IT(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber)
{

}
