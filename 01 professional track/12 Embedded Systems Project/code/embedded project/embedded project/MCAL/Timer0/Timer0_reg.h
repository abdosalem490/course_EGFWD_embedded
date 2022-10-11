/*
 * Timer0_reg.h
 *
 * Created: 25/09/2022 6:58:24 pm
 *  Author: abdo
 */ 


#ifndef TIMER0_REG_H_
#define TIMER0_REG_H_

/*timer0 registers*/
#define TCCR0	*((volatile uint8_t* const)0x53)	/*Timer/Counter Control Register*/
#define TCNT0	*((volatile uint8_t* const)0x52)	/*Timer/Counter Register*/
#define OCR0	*((volatile uint8_t* const)0x5C)	/*Output Compare Register*/
#define TIMSK	*((volatile uint8_t* const)0x59)	/*Timer/Counter Interrupt Mask Register*/
#define TIFR	*((volatile uint8_t* const)0x58)	/*Timer/Counter Interrupt Flag Register Register*/


/*bits related to TCCR0*/
#define TCCR0_FOC0_1BITS		7		/*Force Output Compare*/
#define TCCR0_WGM00_1BITS		6		/*Waveform Generation Mode*/
#define TCCR0_WGM01_1BITS		3		/*Waveform Generation Mode*/
#define TCCR0_COM0_2BITS		4		/*Compare Match Output Mode*/
#define TCCR0_CS0_3BITS			0		/*Clock Select*/

/*bits related to TIMSK*/
#define TIMSK_TOIE0_1BITS		0		/*Output Compare Match Interrupt Enable*/
#define TIMSK_OCIE0_1BITS		1		/*Overflow Interrupt Enable*/

/*bits re;ated to TIFR*/
#define TIFR_TOV0_1BITS			0		/*Output Compare Flag 0*/
#define TIFR_OCF0_1BITS			1		/*Overflow Flag*/

#endif /* TIMER0_REG_H_ */