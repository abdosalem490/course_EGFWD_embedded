/*
 * GPIO_reg.h
 *
 * Created: 24/09/2022 12:32:58 am
 *  Author: abdo
 */ 

 #include "../../utils/standard_int.h"

#ifndef GPIO_REG_H_
#define GPIO_REG_H_

/*register related to GPIO*/
#define PORTA		*((volatile uint8_t* const)0x3B)		/*port A register values*/
#define PORTB		*((volatile uint8_t* const)0x38)		/*port B register values*/
#define PORTC		*((volatile uint8_t* const)0x35)		/*port C register values*/
#define PORTD		*((volatile uint8_t* const)0x32)		/*port D register values*/

#define DDRA		*((volatile uint8_t* const)0x3A)		/*configure the pins direction : input or output*/
#define DDRB		*((volatile uint8_t* const)0x37)		/*configure the pins direction : input or output*/
#define DDRC		*((volatile uint8_t* const)0x34)		/*configure the pins direction : input or output*/
#define DDRD		*((volatile uint8_t* const)0x31)		/*configure the pins direction : input or output*/

#define PINA		*((volatile uint8_t* const)0x39)		/*to read the input to that register*/
#define PINB		*((volatile uint8_t* const)0x36)		/*to read the input to that register*/
#define PINC		*((volatile uint8_t* const)0x33)		/*to read the input to that register*/
#define PIND		*((volatile uint8_t* const)0x30)		/*to read the input to that register*/

/*registers related to external interrupt*/
#define MCUCR		*((volatile uint8_t* const)0x55)		/*MCU Control Register*/
#define MCUCSR		*((volatile uint8_t* const)0x54)		/*MCU Control and Status Register*/
#define GICR		*((volatile uint8_t* const)0x5B)		/*MCU Control and Status Register*/
#define GIFR		*((volatile uint8_t* const)0x5A)		/*General Interrupt Flag Register*/




/*
 *	register related bits
 */

//for MCUCR register
#define MCUCR_ISC0_2BITS			0		/*INT0  Interrupt Sense Control*/
#define MCUCR_ISC1_2BITS			2		/*INT1  Interrupt Sense Control*/

//for MCUCSR register
#define MCUCSR_ISC2_1BITS			6		/*INT2  Interrupt Sense Control*/

//for GICR register
#define GICR_INT0_1BIT				6		/*INT0  External Interrupt Request 1 Enable*/
#define GICR_INT1_1BIT				7		/*INT1  External Interrupt Request 1 Enable*/
#define GICR_INT2_1BIT				5		/*INT2  External Interrupt Request 1 Enable*/

//for GIFR register
#define GIFR_INTF0_1BIT				6		/*INT0  External Interrupt Flag*/
#define GIFR_INTF1_1BIT				7		/*INT1  External Interrupt Flag*/
#define GIFR_INTF2_1BIT				5		/*INT2  External Interrupt Flag*/

#endif /* GPIO_REG_H_ */