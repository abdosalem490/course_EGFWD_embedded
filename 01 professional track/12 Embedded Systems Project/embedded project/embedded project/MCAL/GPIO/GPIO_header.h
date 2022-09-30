/*
 * GPIO_header.h
 *
 * Created: 24/09/2022 12:32:42 am
 *  Author: abdo
 */ 

 #include "../../utils/standard_int.h"

#ifndef GPIO_HEADER_H_
#define GPIO_HEADER_H_


/*
 *	@PINS_NAMES
 */
 #define GPIO_PIN0			0x1
 #define GPIO_PIN1			0x2
 #define GPIO_PIN2			0x4
 #define GPIO_PIN3			0x8
 #define GPIO_PIN4			0x16
 #define GPIO_PIN5			0x32
 #define GPIO_PIN6			0x64
 #define GPIO_PIN7			0x128

/*
 *  @PORTS_NAMES
 */
 #define GPIO_PORTA			0
 #define GPIO_PORTB			1
 #define GPIO_PORTC			2
 #define GPIO_PORTD			3

/*
 *	@PINS_MODES
 */
 #define GPIO_MODE_IN			0
 #define GPIO_MODE_OUT			1	
 #define GPIO_MODE_IN_PULLUP	2

 /*
  *	@PINS_VALUES
  */
 #define PIN_HIGH				1
 #define PIN_LOW				0

/*
 *	@INT_NAMES
 */
 #define GPIO_INT0				0
 #define GPIO_INT1				1
 #define GPIO_INT2				2

/*
 *	@INT_MODES
 * note -: for INT2 , the only modes available are GPIO_INT_MODE_FALL_EDGE and GPIO_INT_MODE_RIS_EDGE
 */
 #define GPIO_INT_MODE_LOW_LEVEL	0
 #define GPIO_INT_MODE_LOGIC_CHANG	1
 #define GPIO_INT_MODE_FALL_EDGE	2
 #define GPIO_INT_MODE_RIS_EDGE		3


 //used to detect incorrect user input
 typedef enum ES_GPIO_t{
	GPIO_OK				=	0,
	GPIO_INVALID_INPUT	=	1,
 }ES_GPIO_t;


 /*
  *	@PUBLIC_FUNCTIONS
  */
ES_GPIO_t GPIO_PinInit(uint8_t args_u8PortName, uint8_t args_u8PinsNumber, uint8_t args_u8Mode);
ES_GPIO_t GPIO_PinsWrite(uint8_t args_u8PortName, uint8_t args_u8PinsNumber, uint8_t args_u8Value);
ES_GPIO_t GPIO_PinRead(uint8_t args_u8PortName, uint8_t args_u8PinNumber, uint8_t *args_u8DataReturn);
ES_GPIO_t GPIO_PinsToggle(uint8_t args_u8PortName, uint8_t args_u8PinsNumber);

ES_GPIO_t GPIO_EnableExtInterrupt(uint8_t args_u8IntNum, uint8_t args_u8IntMode, void (*args_pvCallback)(void));
ES_GPIO_t GPIO_DisableExtInterrupt(uint8_t args_u8IntNum);

#endif /* GPIO_HEADER_H_ */