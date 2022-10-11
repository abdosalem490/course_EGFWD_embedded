/*
 * GPIO_main.c
 *
 * Created: 24/09/2022 12:34:15 am
 *  Author: abdo
 */ 

 #include "GPIO_config.h"
 #include "GPIO_header.h"
 #include "GPIO_private.h"
 #include "GPIO_reg.h"

 #include "../../utils/math_btt.h"
 #include "../atmega32_uPU/atmega32_uPU_header.h"

/*
 *	@fn			-:		-GPIO_PinInit
 *
 *	@params[0]	-:		-symbol representing the port name , options are in header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-symbol representing the pin mode , options are in header file , refer to @PINS_MODES
 *
 *	@brief		-:		-initialize a port pins with a specific mode
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_GPIO_t GPIO_PinInit(uint8_t args_u8PortName, uint8_t args_u8PinsNumber, uint8_t args_u8Mode)
{
	/*local variable to indicate the error state*/
	ES_GPIO_t local_errorState_t = GPIO_OK;

	/*check for the validity of the input*/
	if(args_u8PortName > GPIO_PORTD || args_u8Mode > GPIO_MODE_IN_PULLUP)
	{
		/*input is incorrect*/
		local_errorState_t = GPIO_INVALID_INPUT;
	}
	else
	{
		switch(args_u8PortName)
		{
			case GPIO_PORTA:
			{
				/*the mode is input*/
				if(args_u8Mode == GPIO_MODE_IN)
				{
					DDRA &= ~args_u8PinsNumber;
					PORTA &= ~args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_OUT)
				{
					DDRA |= args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_IN_PULLUP)
				{
					DDRA &= ~args_u8PinsNumber;
					PORTA |= args_u8PinsNumber;
					PINA &= ~args_u8PinsNumber;
				}

				break;
			}

			case GPIO_PORTB:
			{
				/*the mode is input*/
				if(args_u8Mode == GPIO_MODE_IN)
				{
					DDRB &= ~args_u8PinsNumber;
					PORTB &= ~args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_OUT)
				{
					DDRB |= args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_IN_PULLUP)
				{
					DDRB &= ~args_u8PinsNumber;
					PORTB |= args_u8PinsNumber;
					PINB &= ~args_u8PinsNumber;
				}

				break;
			}

			case GPIO_PORTC:
			{
				/*the mode is input*/
				if(args_u8Mode == GPIO_MODE_IN)
				{
					DDRC &= ~args_u8PinsNumber;
					PORTC &= ~args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_OUT)
				{
					DDRC |= args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_IN_PULLUP)
				{
					DDRC &= ~args_u8PinsNumber;
					PORTC |= args_u8PinsNumber;
					PINC &= ~args_u8PinsNumber;
				}

				break;
			}		

			case GPIO_PORTD:
			{
				/*the mode is input*/
				if(args_u8Mode == GPIO_MODE_IN)
				{
					DDRD &= ~args_u8PinsNumber;
					PORTD &= ~args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_OUT)
				{
					DDRD |= args_u8PinsNumber;
				}
				else if(args_u8Mode == GPIO_MODE_IN_PULLUP)
				{
					DDRD &= ~args_u8PinsNumber;
					PORTD |= args_u8PinsNumber;
					PIND &= ~args_u8PinsNumber;
				}

				break;
			}

			default:
			local_errorState_t = GPIO_INVALID_INPUT;
			break;
		}

	}

	/*return the error state*/
	return local_errorState_t;
}


/*
 *	@fn			-:		-GPIO_PinsWrite
 *
 *	@params[0]	-:		-symbol representing the port name , options are in header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-symbol representing the pin value , options are in header file , refer to @PINS_VALUES
 *
 *	@brief		-:		-write a value to a specific pins
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_GPIO_t GPIO_PinsWrite(uint8_t args_u8PortName, uint8_t args_u8PinsNumber, uint8_t args_u8Value)
{
	/*local variable to indicate the error state*/
	ES_GPIO_t local_errorState_t = GPIO_OK;

	/*check for the validity of the input*/
	if(args_u8PortName > GPIO_PORTD)
	{
		/*input is incorrect*/
		local_errorState_t = GPIO_INVALID_INPUT;
	}
	else
	{	
		switch(args_u8PortName)
		{
			case GPIO_PORTA:
			{
				if(args_u8Value == PIN_HIGH)
				{
					PORTA |= args_u8PinsNumber;
				}
				else if(args_u8Value == PIN_LOW)
				{
					PORTA &= ~args_u8PinsNumber;
				}
				else
				{
					local_errorState_t = GPIO_INVALID_INPUT;
				}

				break;
			}			

			case GPIO_PORTB:
			{
				if(args_u8Value == PIN_HIGH)
				{
					PORTB |= args_u8PinsNumber;
				}
				else if(args_u8Value == PIN_LOW)
				{
					PORTB &= ~args_u8PinsNumber;
				}
				else
				{
					local_errorState_t = GPIO_INVALID_INPUT;
				}

				break;
			}

			case GPIO_PORTC:
			{
				if(args_u8Value == PIN_HIGH)
				{
					PORTC |= args_u8PinsNumber;
				}
				else if(args_u8Value == PIN_LOW)
				{
					PORTC &= ~args_u8PinsNumber;
				}
				else
				{
					local_errorState_t = GPIO_INVALID_INPUT;
				}

				break;
			}

			case GPIO_PORTD:
			{
				if(args_u8Value == PIN_HIGH)
				{
					PORTD |= args_u8PinsNumber;
				}
				else if(args_u8Value == PIN_LOW)
				{
					PORTD &= ~args_u8PinsNumber;
				}
				else
				{
					local_errorState_t = GPIO_INVALID_INPUT;
				}

				break;
			}

			default:
			local_errorState_t = GPIO_INVALID_INPUT;
			break;
		}		
	}

	/*return the error state*/
	return local_errorState_t;
}


/*
 *	@fn			-:		-GPIO_PinRead
 *
 *	@params[0]	-:		-symbol representing the port name , options are in header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-a pointer through which the value of the Pin Number will be returned , where it will either return 1 or 0
 *
 *	@brief		-:		-get a read from a pin
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_GPIO_t GPIO_PinRead(uint8_t args_u8PortName, uint8_t args_u8PinNumber, uint8_t *args_u8DataReturn)
{ 
	
	/*local variable to indicate the error state*/
	ES_GPIO_t volatile local_errorState_t = GPIO_OK;

	/*local variable to get the bit number*/
	uint8_t volatile local_u8BitNumber = 0;

	/*local variable used as iterator*/
	uint8_t volatile local_u8Iterator = 1;
	
	/*get the bit number*/
	while(!(local_u8Iterator & args_u8PinNumber))
	{
		local_u8Iterator = local_u8Iterator << 1;
		local_u8BitNumber++;
	}

	/*check for the validity of the input*/
	if(args_u8PortName > GPIO_PORTD)
	{
		/*input is incorrect*/
		local_errorState_t = GPIO_INVALID_INPUT;
	}
	else
	{
		switch(args_u8PortName)
		{
			case GPIO_PORTA:
			{
				*args_u8DataReturn = GET_BIT(PINA, local_u8BitNumber);
				break;
			}			

			case GPIO_PORTB:
			{
				*args_u8DataReturn = GET_BIT(PINB, local_u8BitNumber);
				break;
			}
			
			case GPIO_PORTC:
			{
				*args_u8DataReturn = GET_BIT(PINC, local_u8BitNumber);
				break;
			}
			
			case GPIO_PORTD:
			{
				*args_u8DataReturn = GET_BIT(PIND, local_u8BitNumber);
				break;
			}
						
			default:
			local_errorState_t = GPIO_INVALID_INPUT;
			break;
		}
	}	

	/*return the error state*/
	return local_errorState_t;
}


/*
 *	@fn			-:		-GPIO_PinsToggle
 *
 *	@params[0]	-:		-symbol representing the port name , options are in header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in header file , refer to @PINS_NAMES
 *
 *	@brief		-:		-toggle certain pins
 *
 *	@return		-:		-return error state indicating the validity of the input
 */
ES_GPIO_t GPIO_PinsToggle(uint8_t args_u8PortName, uint8_t args_u8PinsNumber)
{
	/*local variable to indicate the error state*/
	ES_GPIO_t local_errorState_t = GPIO_OK;

	/*check for the validity of the input*/
	if(args_u8PortName > GPIO_PORTD)
	{
		/*input is incorrect*/
		local_errorState_t = GPIO_INVALID_INPUT;
	}
	else
	{
		switch(args_u8PortName)
		{
			case GPIO_PORTA:
			{
				PORTA ^= args_u8PinsNumber;
				break;
			}

			case GPIO_PORTB:
			{
				PORTB ^= args_u8PinsNumber;
				break;
			}

			case GPIO_PORTC:
			{
				PORTC ^= args_u8PinsNumber;
				break;
			}

			case GPIO_PORTD:
			{
				PORTD ^= args_u8PinsNumber;
				break;
			}

			default:
			local_errorState_t = GPIO_INVALID_INPUT;
			break;
		}
	}

	/*return the error state*/
	return local_errorState_t;	
}


/*
 *	@fn			-:		-GPIO_EnableExtInterrupt
 *
 *	@params[0]	-:		-symbol representing the External interrupt name , options are in header file , refer to @INT_NAMES
 *
 *	@params[1]	-:		-symbol representing the interrupt mode , options are in header file , refer to @INT_MODES
 *
 *	@params[2]	-:		-call back function to ISR
 *
 *	@brief		-:		-initialize interrupt and enables it
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_GPIO_t GPIO_EnableExtInterrupt(uint8_t args_u8IntNum, uint8_t args_u8IntMode, void (*args_pvCallback)(void))
{
	/*local variable to indicate the error state*/
	ES_GPIO_t local_errorState_t = GPIO_OK;

	/*validates the input values*/
	if(args_u8IntNum > GPIO_INT2 || args_u8IntMode > GPIO_INT_MODE_RIS_EDGE || (args_u8IntNum == GPIO_INT2 && (args_u8IntMode == GPIO_INT_MODE_LOW_LEVEL || args_u8IntMode == GPIO_INT_MODE_LOGIC_CHANG)))
	{
		local_errorState_t = GPIO_INVALID_INPUT;
	}
	else
	{
		/*enable the global interrupt*/
		ENABLE_GLOBAL_INT();

		/*configure the interrupt mode*/
		switch(args_u8IntNum)
		{
			
			case GPIO_INT0:
			{
				/*configure the interrupt mode*/
				ASSIGN_RANGE(MCUCR, MCUCR_ISC0_2BITS, MCUCR_ISC0_2BITS + 1, args_u8IntMode);

				/*set the call back function*/
				global_pvCallBackINT0 = args_pvCallback;

				/*enable the interrupt*/
				SET_BIT(GICR, GICR_INT0_1BIT);

				break;	
			}

			case GPIO_INT1:
			{
				/*configure the interrupt mode*/
				ASSIGN_RANGE(MCUCR, MCUCR_ISC1_2BITS, MCUCR_ISC1_2BITS + 1, args_u8IntMode);

				/*set the call back function*/
				global_pvCallBackINT1 = args_pvCallback;

				/*enable the interrupt*/
				SET_BIT(GICR, GICR_INT1_1BIT);

				break;
			}

			case GPIO_INT2:
			{
				/*configure the interrupt mode*/
				ASSIGN_RANGE(MCUCSR, MCUCSR_ISC2_1BITS, MCUCSR_ISC2_1BITS + 0, args_u8IntMode);

				/*set the call back function*/
				global_pvCallBackINT2 = args_pvCallback;

				/*enable the interrupt*/
				SET_BIT(GICR, GICR_INT2_1BIT);

				break;
			}

			default:
				local_errorState_t = GPIO_INVALID_INPUT;
			break;
		}

	}

	/*return the error state*/
	return local_errorState_t;
}

ES_GPIO_t GPIO_DisableExtInterrupt(uint8_t args_u8IntNum)
{
	/*local variable to indicate the error state*/
	ES_GPIO_t local_errorState_t = GPIO_OK;

	/*validates the input values*/
	if(args_u8IntNum > GPIO_INT2)
	{
		local_errorState_t = GPIO_INVALID_INPUT;
	}
	else
	{
		switch(args_u8IntNum)
		{
			case GPIO_INT0:
			{
				/*disable the interrupt*/
				CLR_BIT(GICR, GICR_INT0_1BIT);

				break;
			}

			case GPIO_INT1:
			{
				/*disable the interrupt*/
				CLR_BIT(GICR, GICR_INT1_1BIT);

				break;
			}

			case GPIO_INT2:
			{
				/*disable the interrupt*/
				CLR_BIT(GICR, GICR_INT2_1BIT);

				break;
			}

			default:
			local_errorState_t = GPIO_INVALID_INPUT;
			break;
		}		

	}	

	/*return the error state*/
	return local_errorState_t;
}


/************************************************************************/
/* interrupt service routines of the External interrupt                 */
/************************************************************************/

/*
 *	the handler of external interrupt on INT0
 */
void __vector_1(void)
{
	/*call the call back function*/
	global_pvCallBackINT0();

	/*disable the flag of interrupt*/
	SET_BIT(GIFR, GIFR_INTF0_1BIT);

	/*enable the global interrupt after the end of interrupt*/
	ENABLE_GLOBAL_INT();
}

/*
 *	the handler of external interrupt on INT1
 */
void __vector_2(void)
{
	/*call the call back function*/
	global_pvCallBackINT1();

	/*disable the flag of interrupt*/
	SET_BIT(GIFR, GIFR_INTF1_1BIT);

	/*enable the global interrupt after the end of interrupt*/
	ENABLE_GLOBAL_INT();
}

/*
 *	the handler of external interrupt on INT2
 */
 void __vector_3(void)
{
	/*call the call back function*/
	global_pvCallBackINT2();

	/*disable the flag of interrupt*/
	SET_BIT(GIFR, GIFR_INTF2_1BIT);

	/*enable the global interrupt after the end of interrupt*/
	ENABLE_GLOBAL_INT();	
}