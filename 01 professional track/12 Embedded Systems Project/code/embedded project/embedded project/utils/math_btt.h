/*
 * math_btt.h
 *
 * Created: 20/09/2022 10:48:28 pm
 *  Author: abdo
 */ 


#ifndef MATH_BTT_H_
#define MATH_BTT_H_

/*value to be assigned to bit manipulation*/
#define HIGH					1
#define LOW						0

/*bit manipulation*/
#define SET_BIT(VAR, BIT_NUM)						VAR |= (1 << BIT_NUM)																					/*used to set a bit in a variable*/
#define CLR_BIT(VAR, BIT_NUM)						VAR &= ~(1 << BIT_NUM)																					/*used to clear a bit in a variable*/
#define TOG_BIT(VAR, BIT_NUM)						VAR ^= (1 << BIT_NUM)																					/*used to toggle a bit in a variable*/
#define GET_BIT(VAR, BIT_NUM)						((VAR >> BIT_NUM) & 0x01)																				/*used to get a bit in a variable*/
#define ASSIGN_BIT(VAR, BIT_NUM, VAL)				VAR = ((VAR & ~(1 << BIT_NUM)) | (VAL << BIT_NUM))														/*used to assign a bit a specific value*/
								

/*nibble manipulation*/
#define SET_NIB(VAR, NIB_ST)						VAR |= (0x0F << NIB_ST)																				/*used to set a nibble in a variable*/
#define CLR_NIB(VAR, NIB_ST)						VAR &= ~(0x0F << NIB_ST)																				/*used to clear a nibble in a variable*/
#define TOG_NIB(VAR, NIB_ST)						VAR ^= (0x0F << NIB_ST)																				/*used to toggle a nibble in a variable*/
#define GET_NIB(VAR, NIB_ST)						((VAR >> NIB_ST) & 0x0F)																				/*used to get a nibble in a variable*/
#define ASSIGN_NIB(VAR, NIB_ST, VAL)				VAR = ((VAR & ~(0x0F << NIB_ST)) | (VAL << NIB_ST))													/*used to assign a nibble to a specific value*/


/*byte manipulation*/
#define SET_BYTE(VAR, BYTE_ST)						VAR |= (0xFF << BYTE_ST)																				/*used to set a nibble in a variable*/
#define CLR_BYTE(VAR, BYTE_ST)						VAR &= ~(0xFF << BYTE_ST)																				/*used to clear a nibble in a variable*/
#define TOG_BYTE(VAR, BYTE_ST)						VAR ^= (0xFF << BYTE_ST)																				/*used to toggle a nibble in a variable*/
#define GET_BYTE(VAR, BYTE_ST)						((VAR >> BYTE_ST) & 0xFF)																				/*used to get a byte in a variable*/
#define ASSIGN_BYTE(VAR, BYTE_ST, VAL)				VAR = ((VAR & ~(0xFF << BYTE_ST)) | (VAL << BYTE_ST))												/*used to assign a nibble to a specific value*/


/*word manipulation*/
#define SET_WORD(VAR, WORD_ST)						VAR |= (0xFFFF << WORD_ST)																				/*used to set a word in a variable*/
#define CLR_WORD(VAR, WORD_ST)						VAR &= ~(0xFFFF << WORD_ST)																			/*used to clear a word in a variable*/
#define TOG_WORD(VAR, WORD_ST)						VAR ^= (0xFFFF << WORD_ST)																				/*used to toggle a word in a variable*/
#define GET_WORD(VAR, WORD_ST)						((VAR >> WORD_ST) & 0xFFFF)																			/*used to get a word in a variable*/
#define ASSIGN_WORD(VAR, WORD_ST, VAL)				VAR = ((VAR & ~(0xFFFF << WORD_ST)) | (VAL << WORD_ST))												/*used to assign a word to a specific value*/


/*range manipulation*/
#define SET_RANGE(VAR, BIT_START, BIT_END)			VAR |= (((1 << (BIT_END + 1)) - 1) & ~((1 << BIT_START) - 1))											/*used to set a range of bits in a variable*/
#define CLR_RANGE(VAR, BIT_START, BIT_END)			VAR &= ~(((1 << (BIT_END + 1)) - 1) & ~((1 << BIT_START) - 1))											/*used to clear a range of bits in a variable*/
#define TOG_RANGE(VAR, BIT_START, BIT_END)			VAR ^= (((1 << (BIT_END + 1)) - 1) & ~((1 << BIT_START) - 1))											/*used to toggle a range of bits in a variable*/
#define GET_RANGE(VAR, BIT_START, BIT_END)			(VAR >> BIT_START) & ((1 << (BIT_END - BIT_START + 1)) - 1)												/*used to get a range of bits in a variable*/
#define ASSIGN_RANGE(VAR, BIT_START, BIT_END, VAL)	VAR = ((VAR & ~(((1 << (BIT_END - BIT_START + 1)) - 1) << BIT_START)) | (VAL << BIT_START))			/*used to assign a range of bits in a variable*/


#endif /* MATH_BTT_H_ */